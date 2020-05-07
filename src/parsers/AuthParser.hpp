#ifndef _AUTH_PARSER_HPP_
#define _AUTH_PARSER_HPP_

#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
#include <regex>

#include "LogParserInterface.hpp"

namespace LogSupervisor::LogParser{

//! The class parses auth.log files
class Auth : public Interface{
public:
    Auth(const std::string& filePath);
    std::string humanReadableLogType() const override;
    static std::string shortName();
    void parseLog() override;
    std::vector<LogSupervisor::Authentication> all() override;
    virtual ~Auth() {}


private:
    std::ifstream logFile;
    std::vector<LogSupervisor::Authentication> m_auths;

    std::optional<LogSupervisor::Authentication> parseLine(const std::string& line);
    LogSupervisor::Authentication authFromMatches(const std::smatch& matches, bool success);

};

} // namespace

#endif // _AUTH_PARSER_HPP_