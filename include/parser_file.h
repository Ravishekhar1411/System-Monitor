#ifndef PARSER_FILE_H
#define PARSER_FILE_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_parser.h"

using std::string;
using std::vector;

namespace ParserHelper{

template<typename T>

T get_value_of_key(string const & filer, string const & filename)
{
    string key,line;
    T value;
    std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  
    if (filestream.is_open()) 
    {
        while (std::getline(filestream, line)) {
         std::istringstream linestream(line);
            while (linestream >> key >> value)
            {
                if (key == filer) 
                {
                return value;
                }
             }
        }
    }
    return value;
}

};
#endif
