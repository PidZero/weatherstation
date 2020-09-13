// (cc) Johannes Neidhart, 2020
// compile with
// g++ main.cpp -lcurl -std=c++11


#ifndef CURLWRAP_H
#define CURLWRAP_H

#include <sstream>
#include <curl/curl.h>
#include "json.hpp"
#include <iostream>

class curlwrap
{
    public:
        curlwrap();
        ~curlwrap();
        void apicall(std::string &url, std::stringstream &ss);

    private:
        CURL *curl;
        static size_t curl_callback(void*, size_t, size_t, void*);
};


//  Constructor and init of libcurl
inline
curlwrap::curlwrap(){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    this->curl = curl_easy_init();
}


//Destructor
inline
curlwrap::~curlwrap(){
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}


//  The callback function is from the following example:
//  http://troels.arvin.dk/code/examples/curl/curl-example.cpp http://troels.arvin.dk/code/examples/curl/curl-example.cpp
//  It is called when using libcurl to fetch data and
//  uses a stringstream to write the data into memory
inline
size_t curlwrap::curl_callback(void* source_p,size_t size, size_t nmemb, void* dest_p){
    int realsize=size*nmemb;
    std::string chunk((char*)source_p,realsize);
    *((std::stringstream*)dest_p) << chunk;
    return realsize;
}


//  Apicall is the function used most in this class. It will do all
//  the communication. It needs the URL to call and a stringstream, 
//  in which the data from the URL will be stored
inline
void curlwrap::apicall(std::string &url, std::stringstream &ss){
    if (this->curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);        
        curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&ss);
        res = curl_easy_perform(this->curl);
    }
}

#endif
