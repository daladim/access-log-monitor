#ifndef _LOG_PARSER_INTERFACE_HPP_
#define _LOG_PARSER_INTERFACE_HPP_

#include <iostream>
#include <vector>

#include "../database/DatabaseObjects.hpp"

namespace LogSupervisor::LogParser{

//! The interface every log parser should comply to
class Interface{
public:
    //! Human-readable string that describes the type of logs this type of parser operates on
    virtual std::string humanReadableLogType() const = 0;
    //! A short name that the user can type (e.g. as a command-line argument)
    static std::string shortName() = delete;

    //! Parse a log, the path of which was given in the constructor
    virtual void parseLog() = 0;

    //! Retrieve the Authentication objects that have been generated when parsing the log file
    virtual std::vector<LogSupervisor::Authentication> all() = 0;

    virtual ~Interface(){};
};

} // namespace

#endif //_LOG_PARSER_INTERFACE_HPP_