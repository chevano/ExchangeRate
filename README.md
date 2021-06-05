# ExchangeRate

This project creates an exchange rate program that will fetch the most recent exchange rates for different currency and report them to the user.

In order to do this, the user is asked to enter the currency code in which he/she would like to compare against.

This project uses the ExchangeRate API found at https://app.exchangerate-api.com/

This project consist of two external libraries, namely lib curl  nlohmann/json(JSON for modern C++).

The lib curl library is use to make web calls to a JSON API

libcurl url:
http://curl.haxx.se/

libcurl is a C library, not a C++ library, therefore in order to make it C++ friendly the C++ STL utilities are used.

nlohmann/json can be found here:
http://github.com/nlohmann/json

This is a C++11 style JSON parsing and building library that is availble as a single header file.

Configuration
-------------

Note: If you are on a Mac you should have curl already instead to check Run man curl,
If not then download it at the link below.

Go to http://curl.haxx.se/download.html
Scroll down and look on the right hand side and you will see a list of Operation System.
Download the curl for your Operating System

The steps below tells you how to retrieve the json.hpp file but for simplicity I uploaded the file to this repository.

Go to http://github.com/nlohmann/json
Click on single_include/nlohmann
Click on json.hpp
Click raw and copy the contents and place it inside your own json.hpp file
Clone the repository and store it in the same directory where you plan on storing your cpp files for this project.

How to Run the Program
----------------------
Step 1: !g++
Step 2: g++ --std=c++17 -Wall -Wextra ExchangeRate.cpp -o ExchangeRate -lcurl
