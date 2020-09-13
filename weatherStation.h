// (cc) Johannes Neidhart, 2020
// compile with
// g++ main.cpp -lcurl -std=c++11


#include <sstream>
#include <fstream>
#include <curl/curl.h>
#include "json.hpp"
#include "curlwrap.h"
#include <time.h>
#include <vector>


#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

class weatherstation
{
    public:
        weatherstation(std::string  town, std::string owmId);
        ~weatherstation();

        void pull_weatherdata();
        void pull_forecastdata();
        void pull_time();

        std::string year;
        std::string month;
        std::string day;
        std::string time;

        std::string temperature;
        std::string description;

        std::vector <std::string> fc_desc;
        std::vector <std::string> fc_temp;
        std::vector <std::string> fc_times;

    private:
        curlwrap curl;
        std::stringstream ss;
                nlohmann::json weatherdata;
        nlohmann::json forecastdata;
        nlohmann::json timedata;
        std::string weatherurl;
        std::string forecasturl;
        std::string timeurl{"http://worldtimeapi.org/api/ip"};
};


//Constructor needs the name of a town and an open weather map ID
inline
weatherstation::weatherstation(std::string town, std::string owmId){
    fc_desc.resize(3);
    fc_temp.resize(3);
    fc_times.resize(3);
    weatherurl = "https://api.openweathermap.org/data/2.5/weather?q=" + town + "&appid=" + owmId + "&units=metric";
    forecasturl = "https://api.openweathermap.org/data/2.5/forecast?q=" + town + "&cnt=5&appid=" + owmId + "&units=metric";
}


//Destructor
inline 
weatherstation::~weatherstation(){
}


//Pull weatherdata and write it to weatherdata, also extract temperature and description
inline
void weatherstation::pull_weatherdata(){
    // to change the type later on it is easier to work with a buffer
    double buff;
    // use curl to call the api and get the weatherdata, it will be written to the stringstream ss
    curl.apicall(weatherurl, ss);

    // due to the limited number of allowed calls for the free version of the API, 
    // it can be a good idea to store an example output for further developement
    //std::ifstream examplefile ("weatherexample.txt");
    //ss << examplefile.rdbuf();
    //examplefile.close();

    // write the contents of ss to the nlohmann::json weatherdata
    ss>>weatherdata;

    // It is convenient for output to be stored as string. For formatting and typeconversion it is easier to 
    // work with a buffer because temperature is stored as a floating point number:
    buff = weatherdata["main"]["temp"];
    buff = std::round(buff); 
    temperature = std::to_string(int(buff));

    // The weatherdescription is stored as a string, so we can write it directly to the descripion variable
    description = weatherdata["weather"][0]["description"];
}


//Pull the forecastdata and write it to forecastdata
inline
void weatherstation::pull_forecastdata(){
    std::string timebuf;
    std::string buf;
    std::stringstream ssb;

    double buff;
    curl.apicall(forecasturl, ss);
    //std::ifstream examplefile ("forecastexample.txt");
    //ss << examplefile.rdbuf();
    //examplefile.close();
    ss>>forecastdata;

    // extracting parts from a JSON to a buffer:
    ssb<<forecastdata["list"][1]["dt_txt"];
    std::getline(ssb, buf, ' ');
    std::getline(ssb, timebuf, ':');
    if(timebuf.at(0) == '0') timebuf.erase(timebuf.begin());
    fc_times.at(0) = timebuf;
    ssb.clear();
    ssb<<forecastdata["list"][2]["dt_txt"];
    std::getline(ssb, buf, ' ');
    std::getline(ssb, timebuf, ':');
    if(timebuf.at(0) == '0') timebuf.erase(timebuf.begin());
    fc_times.at(1) = timebuf;
    ssb.clear();
    ssb<<forecastdata["list"][3]["dt_txt"];
    std::getline(ssb, buf, ' ');
    std::getline(ssb, timebuf, ':');
    if(timebuf.at(0) == '0') timebuf.erase(timebuf.begin());
    fc_times.at(2) = timebuf;

    // It is convenient for all output to be stored as string. For formatting and typeconversion it is easier to 
    // work with a buffer because temperature is stored as a floating point number:
    buff = forecastdata["list"][1]["main"]["temp"];
    buff = std::round(buff); 
    fc_temp.at(0) = std::to_string(int(buff));

    buff = forecastdata["list"][2]["main"]["temp"];
    buff = std::round(buff); 
    fc_temp.at(1) = std::to_string(int(buff));

    buff = forecastdata["list"][3]["main"]["temp"];
    buff = std::round(buff); 
    fc_temp.at(2) = std::to_string(int(buff));

    // The weatherdescription is stored as a string, so we can write it directly to the descripion variable
    fc_desc.at(0) = forecastdata["list"][1]["weather"][0]["description"];
    fc_desc.at(1) = forecastdata["list"][2]["weather"][0]["description"];
    fc_desc.at(2) = forecastdata["list"][3]["weather"][0]["description"];

}


//Pull current time and write it to timedata and extract time and date
inline
void weatherstation::pull_time(){
    // Use the curl wrapper to call the API and write the data to ss
    curl.apicall(timeurl, ss);

    // store the contents of ss in the json timedata
    ss>>timedata; 

    // declare some buffers
    std::string buffer;
    std::string yearbuffer;
    std::stringstream ssb, ssy;

    // extract the timedata from the json and get rid of unwanted symbols
    ssb<<timedata["datetime"];
    std::getline(ssb, buffer, '"');
    std::getline(ssb, yearbuffer, 'T');
    ssy<<yearbuffer;

    // extract the time and date from the prepared strings
    std::getline(ssb, time, '.');
    std::getline(ssy, year, '-');
    std::getline(ssy, month, '-');
    std::getline(ssy, day, '-');
}
#endif
