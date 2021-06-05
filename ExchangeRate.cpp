#include <iostream>
#include <memory>
#include <functional>
#include <cstdio>
#include <array>
#include <algorithm>
#include <string>

#include <curl/curl.h>
#include "json.hpp"


// The default behavior of curl is to print the raw data to the console.
// In an effort to correct this behavior a custom writer function 
// is declared to capture the data being fetch by the Bitcoin class into the program
// rather than the console so that I can control how the data gets printed.

   extern "C" std::size_t dataHandler(const char* buffer, std::size_t size, std::size_t nmemb, std::string* userData) {
      if(userData == nullptr)
         return 0;

      userData->append(buffer, (size * nmemb));

      // The number of bytes being written
      return size * nmemb;
   }

// Unique pointer CURL_ptr holds a reference to a curl object and a deleter
// function that takes in a Curl pointer whose return type is void 

typedef std::unique_ptr<CURL, std::function<void(CURL*) >> CURL_ptr;


// Recall a Curl object in C is a void pointer
class CurlHandler {
   private:
      std::string data;
      CURL_ptr curl_ptr;

      // Custom deleter, needed to handle the disposal of a void pointer.
      // Since unique pointers and CURL objects are not compatible
      // Recall a CURL Object in C programming language is a void pointer
      // The unique pointer container does not know how to release the memory of a void 
      // pointer, since it can be pointing to anything therefore this deleter solves the
      // issue of deallocating a curl object.
      // This function will be called every time the curl object goes out of scope.

      constexpr static auto deleter = [](CURL* theCurl) {
         curl_easy_cleanup(theCurl);
         curl_global_cleanup();
      };

   public:
      // Default Constructor
      CurlHandler() : curl_ptr(curl_easy_init(), deleter) {
         // Initialize all the memory needed for the library to work properly
         curl_global_init(CURL_GLOBAL_ALL);
         // Attach the custom writer to the curl library
         curl_easy_setopt(curl_ptr.get(), CURLOPT_WRITEFUNCTION, dataHandler);
         curl_easy_setopt(curl_ptr.get(), CURLOPT_WRITEDATA, &data);
      }

      // Tells the curl library which url you want to get the data from
      void setUrl(std::string url) {
         curl_easy_setopt(curl_ptr.get(), CURLOPT_URL, url.c_str());
      }

      // Retrieve the requested data
      CURLcode fetch() {
         return curl_easy_perform(curl_ptr.get());
      }

      // returns the fetch data from the program
      std::string getData() {
         return data;
      }
};

class ExchangeRate {
   using json = nlohmann::json;

   private:
      CurlHandler curlHandler;
      std::string currencyCode;
      std::string API;

   public:
      ExchangeRate() : curlHandler({}) {

      }

      json fetchExchangeRateData() {
         curlHandler.fetch();

         return json::parse(curlHandler.getData());
      }

      void setCurrencyCode(std::string currencyCode) {
         this->currencyCode = currencyCode;
         API = "https://v6.exchangerate-api.com/v6/c31d6b38f8eb94f597cba814/latest/" + currencyCode;
         curlHandler.setUrl(API);
      }

      std::string getCurrencyCode() {
         return currencyCode;
      }
};

std::string replaceAll(std::string &originalString, std::string stringToBeReplaced, std::string replacementString) {
   std::size_t pos;
   std::string str = "";

   while( (pos = originalString.find(stringToBeReplaced)) != std::string::npos) {
      originalString.replace(pos, 1, replacementString);
   }
   return originalString;
}

int main() {

   using namespace std;
   using json = nlohmann::json;

   // set the default to US currency
   string currencyCode = "USD";

   cout << "Please enter your the 3 character currency code you wish to check against" << endl;
   cin >> currencyCode;

   try {
      ExchangeRate exchangeRate;
      std::transform(currencyCode.begin(), currencyCode.end(), currencyCode.begin(), ::toupper);
      exchangeRate.setCurrencyCode(currencyCode);
      json exchangeRateData = exchangeRate.fetchExchangeRateData();

      // Converts json object to string 
      string data = exchangeRateData["conversion_rates"].dump();

      replaceAll(data, "{", "");
      replaceAll(data, "}", "");
      replaceAll(data, "," , "\n");

      cout << "1 " + currencyCode + " is equivalent to the following rates: \n\n";
      cout << data << endl;

   } catch(...) {
     cerr << "Failed to fetch data\n"; 
   }

   return 0;
}