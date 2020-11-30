#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"
#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

  //get all of processes using helper function
  processes_.clear();
  for( int& id : LinuxParser::Pids())
  {
   // std::cout<<'\n'<<'\r'<< "Process Id :  "<<id<<'\n';
    Process process(id);
    processes_.push_back(process); 
  }

  std::sort(processes_.begin(),processes_.end());
  //std::cout<<'\n'<<'\r'<< "Size of Process vector is : "<< this->processes_.size();
  return processes_; 

}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    
   //  std::cout <<"System Kernel is : "<< LinuxParser::Kernel(); 
     return LinuxParser::Kernel(); 
   //  return string();
     }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    //return string();
    return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  //std::cout<< "System::UpTime() :"<<LinuxParser::UpTime()<<'\n';
  return LinuxParser::UpTime(); }
