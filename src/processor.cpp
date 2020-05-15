#include "processor.h"
#include "linux_parser.h"

Processor::Processor(){
    last_jiffies = LinuxParser::Jiffies();
    last_idle_jiffies = LinuxParser::IdleJiffies();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float current_jiffies = LinuxParser::Jiffies();
    float current_idle_jiffies = LinuxParser::IdleJiffies();
    float diff = current_jiffies - last_jiffies;
    float idle_diff = current_idle_jiffies - last_idle_jiffies;
    float utilization = (diff - idle_diff) / diff;
    last_jiffies = current_jiffies;
    last_idle_jiffies = current_idle_jiffies;
    return utilization;
}