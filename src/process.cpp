#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include <iostream>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return this->_pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization; }

// TODO: Return the command that generated this process
string Process::Command() { 
    
    // string line,key,value,kb;
    //  // File open /proc/meminfo
    // std::ifstream filestream (LinuxParser::kProcDirectory + std::to_string(this->_pid) +LinuxParser::kCmdlineFilename);
  
    // if(filestream.is_open())
    // {
    //     std::getline(filestream, line);
    //     std::istringstream linestream(line);
    //     linestream >> key;
    // }
    //     return key; 
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 

  // string line,key,value,kb;
  // unsigned long in_MB;

  // // File open /proc/meminfo
  // std::ifstream filestream (LinuxParser::kProcDirectory + std::to_string(this->_pid) +LinuxParser::kStatusFilename);
  
  // if(filestream.is_open())
  // {
  //     while(std::getline(filestream, line))
  //     {
  //       std::istringstream linestream(line);
  //       linestream >> key >>value;
  //       if(key == "VmSize:")
  //       {
  //         // std::cout<< " VmSize : "<< value;
  //           break;
  //       }
  //     }
  // }
  // filestream.close();
  // // Converting string to unsigned long in MB 
  //   in_MB = stoul(value)/1000;
  //   return to_string(in_MB); 

  return ram;
    
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    
  //   string line,key,value;

  //   // File open /proc/meminfo
  //   std::ifstream filestream (LinuxParser::kProcDirectory + std::to_string(this->_pid) +LinuxParser::kStatusFilename);
  
  //   if(filestream.is_open())
  //   {
  //       while(std::getline(filestream, line))
  //       {
  //           std::istringstream linestream(line);
  //           linestream >> key >>value;
  //           if(key == "Uid:")
  //           {
  //         //  std::cout<< " Uid: "<< value;
  //           break;
  //           }
  //     }
  // }
  //   return value; 
  return user;
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return upTime;
 }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
//bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
bool Process::operator<(Process const &a) const {
  if (cpu_utilization > a.cpu_utilization)
    return true;
  else
    return false;
}

  Process::Process(int pid) {
  _pid = pid;
   user = LinuxParser::User(_pid);
   command = LinuxParser::Command(_pid);
   cpu_utilization = LinuxParser::CpuUtilization(_pid);
   ram = LinuxParser::Ram(_pid);
   upTime = LinuxParser::UpTime(_pid);
}