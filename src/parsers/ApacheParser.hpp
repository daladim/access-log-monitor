#ifndef _APACHE_PARSER_HPP_
#define _APACHE_PARSER_HPP_

#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
#include <regex>

#include "LogParserInterface.hpp"

namespace LogSupervisor::LogParser{

//! The class parses Apache "combined" log files
class Apache : public Interface{
public:
    Apache(const std::string& filePath);
    std::string humanReadableLogType() const override;
    void parseLog() override;
    std::vector<LogSupervisor::Authentication> all() override;
    virtual ~Apache() {}

private:
    std::ifstream logFile;
    std::vector<LogSupervisor::Authentication> m_auths;

    std::optional<LogSupervisor::Authentication> parseLine(const std::string& line);
    LogSupervisor::Authentication authFromMatches(const std::smatch& matches, bool success);

};

} // namespace

#endif // _APACHE_PARSER_HPP_