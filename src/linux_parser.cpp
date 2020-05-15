#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

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
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  string key;
  float value;
  float total_memory = -1;
  float free_memory = -1;

  string line;
  while(((total_memory == -1) || (free_memory == -1)) && getline(filestream, line)){
    std::istringstream stringstream(line);
    stringstream >> key >> value;
    if(key == "MemTotal:"){
      total_memory = value;
    }else if(key == "MemFree:"){
      free_memory = value;
    }
  }
  return (total_memory - free_memory) / total_memory; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  string uptime;
  filestream>>uptime;
  return std::stol(uptime); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  long jiffies = 0;
  string line;
  string cpu;
  string stat;
  while(getline(filestream, line)){
    std::istringstream stringstream(line);
    stringstream>>cpu;
    int count = 0;
    while(count++ < 10 && stringstream >> stat){
      jiffies += std::stol(stat);
    }
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  long active_jiffies = 0;
  string s;
  if(filestream.is_open()){
    for(int i = 0; i < 14; i++)
      filestream >> s;
    for(int i = 0; i < 4; i++){
      filestream >> s;
      active_jiffies += std::stol(s);
    }
  }
  return active_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  long active_jiffies = 0;
  string s;
  if(filestream.is_open()){
    filestream >> s;
    for(int i = 0; i < 10; i++){
      filestream >> s;
      active_jiffies += std::stol(s);
    }
  }
  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  long idle_jiffies = 0;
  string s;
  if(filestream.is_open()){
    for(int i = 0; i < 4; i++)
      filestream >> s;  
    for(int i = 0; i < 2; i++){
      filestream >> s;
      idle_jiffies += std::stol(s);
    }
  }
  return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  long old_active = ActiveJiffies();
  long old_idle = IdleJiffies();
  sleep(5);
  long new_active = ActiveJiffies();
  long new_idle = IdleJiffies();
  long active_diff = new_active - old_active;
  long idle_diff = new_idle - old_idle;
  string utilization =  Format::ElapsedTime((active_diff - idle_diff) / active_diff);
  return {utilization}; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string key, val, line;
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream stringstream(line);
      stringstream >> key >> val;
      if(key == "processes")
        return std::stoi(val);
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string key, val, line;
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream stringstream(line);
      stringstream >> key >> val;
      if(key == "procs_running")
        return std::stoi(val);
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    string line;
    getline(filestream, line);
    return line;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string key, val, line;
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream stringstream(line);
      stringstream >> key >> val;
      if(key == "VmData:"){
        int temp = (100 * std::stoi(val)) / 1024;
        return to_string(temp / 100) + "." + to_string(temp % 100);
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string key, val, line;
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream stringstream(line);
      stringstream >> key >> val;
      if(key == "Uid:")
        return val;
    }
  }
  return string();

}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream filestream(kPasswordPath);
  string line;
  string token = "x:" + to_string(pid);
  while(getline(filestream, line)){
    int index = line.find(token);
    if(index != -1){
      return line.substr(0, index - 1);
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  string s;
  if(filestream.is_open()){
    for(int i = 0; i < 14; i++)
      filestream >> s;  
    return std::stol(s);
  }
  return 0; 
}