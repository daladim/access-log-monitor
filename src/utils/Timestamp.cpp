#include <iostream>
#include <ctime>
#include <cstring>
using namespace std;

#include "Timestamp.hpp"

namespace LogSupervisor{

Timestamp::Timestamp(const std::chrono::system_clock::time_point& chronoTs) :
    tp(chronoTs)
{}

Timestamp::Timestamp(const std::string& time, const char* fmt) :
    Timestamp(time.c_str())
{}

Timestamp::Timestamp(const char* time, const char* fmt){
    std::tm tm = {};
    strptime(time, fmt, &tm);
    tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
}


Timestamp Timestamp::NOW(){
    return Timestamp(std::chrono::system_clock::now());
}

string Timestamp::to_string(const char* fmt) const{
    // Let's use terrible C APIs while std::chrono is getting standardized...
    const int buffer = 30;
    char *sz = (char*)malloc(buffer);
    std::time_t now_c = std::chrono::system_clock::to_time_t(tp);
    std::tm* now_tm = std::localtime(&now_c);
    strftime(sz, buffer-1, fmt, now_tm);
    return string(sz);
}

} // namespace