#ifndef _LOG_PARSER_INTERFACE_HPP_
#define _LOG_PARSER_INTERFACE_HPP_

#include <iostream>

#include "../database/DatabaseObjects.hpp"

namespace LogSupervisor::LogParser{

//! The interface every log parser should comply to
class Interface{
public:
    //! Parse a log, the path of which was given in the constructor
    virtual void parseLog() = 0;

    //! Retrieve the Authentication objects that have been generated when parsing the log file
    virtual const std::vector<LogSupervisor::Authentication> all() const = 0;
};

} // namespace

#endif //_LOG_PARSER_INTERFACE_HPP_