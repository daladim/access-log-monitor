#ifndef _TIMESTAMP_HPP_
#define _TIMESTAMP_HPP_

#include <chrono>

namespace LogSupervisor{

//! This represents a single time point
class Timestamp{
public:
    static constexpr char DEFAULT_FORMAT[] = "%Y-%m-%d %H:%M:%S";

    //! Returns the current time
    static Timestamp NOW();

    //! Construct from a std::chrono timestamp
    Timestamp(const std::chrono::system_clock::time_point& chronoTs);

    //! Construct from a string
    Timestamp(const std::string& time, const char* fmt = DEFAULT_FORMAT);
    //! Construct from a string
    Timestamp(const char* time, const char* fmt = DEFAULT_FORMAT);

    //! A SQL-compatible string representation of the Timestamp
    std::string to_string(const char* fmt = DEFAULT_FORMAT) const;

private:
    std::chrono::system_clock::time_point tp;
};

} // namespace

#endif //_TIMESTAMP_HPP_