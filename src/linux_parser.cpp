#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>

// User define libraries for testing and debugging
#include "linux_parser.h"
#include "parser_file.h"
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  float Mem_Total,Mem_Free;
  Mem_Total = ParserHelper::get_value_of_key<int>("MemTotal:",kMeminfoFilename);
  Mem_Free = ParserHelper::get_value_of_key<int>("MemFree:",kMeminfoFilename);
  return((Mem_Total-Mem_Free)/Mem_Total); 
 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
   
  string sLine,uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if (stream.is_open()) {
    // Call getline to get first line and just parse first token (uptime)
    std::getline(stream, sLine);
    std::istringstream linestream(sLine);
    linestream >> uptime;
  }
  stream.close();
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
    std::string line,temp;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    long cpu_jiffies[10],index = 0;
    if (stream.is_open()) {
        std::getline(stream, line);
        std::stringstream linestream(line);
        std::cout<< "ActiveJiffies()  : "<<'\n';
        while(linestream>>temp)
        {
            if(temp == "cpu")
                continue;
            
            cpu_jiffies[index++]= std::stoll(temp);
        }
    }


  return cpu_jiffies[CPUStates::kUser_]+ cpu_jiffies[CPUStates::kNice_]+ cpu_jiffies[CPUStates::kSystem_]+cpu_jiffies[CPUStates::kIRQ_]+cpu_jiffies[CPUStates::kSoftIRQ_]
  +cpu_jiffies[CPUStates::kSteal_];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
    std::string line,temp;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    long cpu_jiffies[10],index = 0;
    if (stream.is_open()) {
        std::getline(stream, line);
        std::stringstream linestream(line);
        std::cout<< "IdleJiffies()  : "<<'\n';
        while(linestream>>temp)
        {
            if(temp == "cpu")
                continue;
            
            cpu_jiffies[index++]= std::stoll(temp);
        }
    }
  return cpu_jiffies[CPUStates::kIdle_]+cpu_jiffies[CPUStates::kIOwait_]; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

float LinuxParser::CpuUtilization(int pid)
{

  try {
    std::stringstream ss;
    ss << kProcDirectory << pid << kStatFilename;

    std::stringstream ss_uptime;
    ss_uptime << kProcDirectory << kUptimeFilename;

    long hertz = sysconf(_SC_CLK_TCK);

    string utime = GetPropertyFromFile(ss.str(), 13);
    string stime = GetPropertyFromFile(ss.str(), 14);
    string cutime = GetPropertyFromFile(ss.str(), 15);
    string cstime = GetPropertyFromFile(ss.str(), 16);
    string starttime = GetPropertyFromFile(ss.str(), 21);
    string uptime = GetPropertyFromFile(ss_uptime.str(), 0);

    long lutime = stol(utime);
    long lstime = stol(stime);
    long lcutime = stol(cutime);
    long lcstime = stol(cstime);
    long lstarttime = stol(starttime);
    long luptime = stol(uptime);

    long total_time = lutime + lstime;
    total_time = total_time + lcutime + lcstime;
    float seconds = luptime - (lstarttime / hertz);
    float cpu_usage = ((total_time / hertz) / seconds);

    if (cpu_usage > 1) cpu_usage = 0;

    return cpu_usage;

  } catch (...) {
    return 0;
  }
}
  

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  return ParserHelper::get_value_of_key<int>("processes",kStatFilename);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  return ParserHelper::get_value_of_key<int>("procs_running",kStatFilename);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    std::string line,temp,command,discard;
    std::ifstream stream(LinuxParser::kProcDirectory +to_string(pid) + LinuxParser::kCmdlineFilename);
    
    if (stream.is_open())
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream>>command>>discard;
    }        
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 

  long int mem = ParserHelper::get_value_of_key<long int>("VmSize:",to_string(pid)+kStatusFilename);
  return to_string(mem/1024);

 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  return ParserHelper::get_value_of_key<string>("Uid:",to_string(pid)+"/status");
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::  User(int pid) { 
 
  string uid = Uid(pid);

  string line, user;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line))  // Read a whole line per cycle
    {
      line = StringReplace(&line, ':', ' ');
      std::istringstream linestream(line);
      using StrIt = std::istream_iterator<std::string>;
      std::vector<std::string> container{StrIt{linestream}, StrIt{}};
      if (container.at(2).compare(uid) == 0) {
        return container.at(0);
      }
    }
  }
  return uid;
  
 /*
   string ptrn{string(":x:").append(Uid(pid))};
  // std::cout<<" UID for PID "<<pid<< "==" <<Uid(pid)<<'\n';
  string line{""};
  string user{""};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(ptrn) != string::npos) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> user;
      }
    }
  }
  filestream.close();
  return user;
  */
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  // std::stringstream ss;
  // ss << kProcDirectory << pid << kStatFilename;
  //  //  std::cout << " File path is "<<kProcDirectory+to_string(pid)+kUptimeFilename<<'\n';
  // string uptime_str = GetPropertyFromFile(ss.str(),21);
  // long uptime_clock_ticks= stoi(uptime_str);
  // // converted the time value to seconds
  // long up_time_in_second = uptime_clock_ticks/ sysconf(_SC_CLK_TCK);
  // return up_time_in_second; 
    try {
    std::stringstream ss;
    ss << kProcDirectory << pid << kStatFilename;
    std::string property = GetPropertyFromFile(ss.str(), 21);
    if (!property.empty()) {
      long secs = stoi(property);
      long hertz = sysconf(_SC_CLK_TCK);
      return secs / hertz;
    }
  } catch (...) {
    return 0;
  }
  return 0;
  }

std::string LinuxParser::GetPropertyFromFile(std::string filePath,
                                             std::string propertyName) {
  string line, property, desc;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    while (std::getline(stream, line))  // Read a whole line per cycle
    {
      std::istringstream linestream(line);
      linestream >> desc >> property;
      if (desc == propertyName) {
        return property;
      }
    }
  }
  return {};
}

// 2nd overloaded Implementation with the position passed as the search
// parameter
std::string LinuxParser::GetPropertyFromFile(std::string  filePath,unsigned long position) 
{
    std::string line,temp;
    std::vector<string> File_content_vector; 
    std::ifstream stream(filePath);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::stringstream linestream(line);     
        // Saving inout stream into vector to find position value 
        copy(std::istream_iterator<string>(linestream), std::istream_iterator<string>(),back_inserter(File_content_vector));
        if(File_content_vector.size()>= position+1)
          return File_content_vector.at(position);

    } 
    return {};

}

string LinuxParser::StringReplace(string *input, char oldChar, char newChar) {
  std::transform(input->begin(), input->end(), input->begin(),
                 [oldChar, newChar](char c) {
                   if (c != oldChar) {
                     return c;
                   } else {
                     return newChar;
                   }
                 });
  return *input;
}