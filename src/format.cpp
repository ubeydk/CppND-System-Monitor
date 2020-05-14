#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    const long SECOND_IN_HOUR = 3600;
    const long SECOND_IN_MIN = 60;
    string hour = std::to_string(seconds / SECOND_IN_HOUR);
    seconds %= SECOND_IN_HOUR;
    string minute = std::to_string(seconds / SECOND_IN_MIN);
    return hour + ":" + minute + ":" + std::to_string(seconds % SECOND_IN_MIN);
}