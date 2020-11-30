#include "processor.h"
#include <iostream>
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    
    unsigned long int total_cpu_time = 0;
    float idle_percentage =0.0,active_time =0 ;
    std::string line,temp;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::stringstream linestream(line);
       // std::cout<< "Processor::Utilization() : "<< line <<'\n';
        while(linestream>>temp)
        {
            if(temp=="cpu")
                continue;
            this->cpu.push_back(std::stoll(temp));
        }

        for(unsigned int i = 0; i < this->cpu.size(); i++)
            total_cpu_time += this->cpu[i];
        
        idle_percentage = float(this->cpu[3])/float(total_cpu_time);
        active_time = (1.0-idle_percentage);
        
    }
    return active_time;
    


}