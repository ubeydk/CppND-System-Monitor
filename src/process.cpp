#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    pid_ = pid;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long current_process_tick = LinuxParser::ActiveJiffies(pid_);
    long current_system_tick = LinuxParser::Jiffies();
    long process_duration = current_process_tick - last_process_tick;
    long system_duration = current_system_tick - last_system_tick;
    last_system_tick = current_system_tick;
    last_process_tick = current_process_tick;
    cpu_utilization_ = ((float)process_duration) / system_duration;
    return cpu_utilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return cpu_utilization_ < a.cpu_utilization_;
}