// (cc) Johannes Neidhart, 2020
// compile with
// g++ main.cpp -lcurl -std=c++11


#include <stdio.h>
#include "weatherStation.h"


int main() {
    weatherstation ws("<your place>", "<your ID>");
    ws.pull_time();
    ws.pull_forecastdata();
    ws.pull_weatherdata();
    std::cout<<ws.day<<"."<<ws.month<<"."<<ws.year<<"\t"<<ws.time<<std::endl;
    std::cout<<"Right now: "<<ws.description<<" at "<<ws.temperature<<"°C"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"The weather today:"<<std::endl;
    std::cout<<ws.fc_times.at(0)<<"h:\t"<<ws.fc_desc.at(0)<<"\t"<<ws.fc_temp.at(0)<<"°C"<<std::endl;
    std::cout<<ws.fc_times.at(1)<<"h:\t"<<ws.fc_desc.at(1)<<"\t"<<ws.fc_temp.at(1)<<"°C"<<std::endl;
    std::cout<<ws.fc_times.at(2)<<"h:\t"<<ws.fc_desc.at(2)<<"\t"<<ws.fc_temp.at(2)<<"°C"<<std::endl;
    return 0;
}
