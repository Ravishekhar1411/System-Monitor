#include <string>

#include "format.h"

#include <math.h>

#include <iomanip>
#include <iostream>


using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 

   time_t rawtime = seconds;
   struct tm *info;
   info = gmtime(&rawtime );   
   std::stringstream ss;
   
    ss <<std::setfill('0') <<std::setw(2)<<info->tm_hour << ":" << std::setfill('0') <<std::setw(2)<< info->tm_min << ":" << std::setfill('0') <<std::setw(2)<<info->tm_sec;
    string time_obs_def = ss.str();
	return time_obs_def;
	}