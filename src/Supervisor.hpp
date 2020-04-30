#ifndef _LOG_SUPERVISOR_HPP_
#define _LOG_SUPERVISOR_HPP_

#include <iostream>

#include "utils/LogParserInterface.hpp"
#include "database/Database.hpp"
#include "database/DatabaseObjects.hpp"
#include "Config.hpp"

namespace LogSupervisor{

//! The class does all the logic
class Supervisor{
public:
    //! Main constructor
    Supervisor(const Config& config, LogParser::Interface* logParser);

    //! Run the main process.<br>
    //! This parses the log, checks the validity of the authentications, and store the result into a database<br>
    //! See database() to retrieve the results
    void run();

    //! Retrieve the database of classified authentications
    const Database::Database& database() const{ return db; }

private:
    const Config config;
    LogParser::Interface* logParser;
    Database::Database db;
};

} // namespace

#endif // _LOG_SUPERVISOR_HPP_