#include "curlwrap.h"
#include <string.h>
#include <sstream>
#include <stdio.h>

int main(){
    curlwrap cw;
    std::stringstream back; 
    std::string timeurl="http://worldtimeapi.org/api/ip";
    cw.apicall(timeurl, back);
    std::cout<<back.str()<<std::endl;
    return(1);
}
