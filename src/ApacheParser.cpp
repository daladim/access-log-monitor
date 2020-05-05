#include "ApacheParser.hpp"

#include <string>
using namespace std;

namespace LogSupervisor::LogParser{

Apache::Apache(const string& filePath) :
    logFile(filePath)
{}

void Apache::parseLog(){
    string currentLine;
    while(std::getline(logFile, currentLine)){
        optional<Authentication> auth = parseLine(currentLine);
        if(auth){
            m_auths.push_back(*auth);
        }
    }

}

optional<LogSupervisor::Authentication> Apache::parseLine(const std::string& line){
    //                          IP                 user
    regex re_apache(R"raw(^\s?([^\s]+)\s+[^\s]+\s+([^\s]+)\s+\[.+)raw");

    smatch matches;
    if(regex_search(line, matches, re_apache)){
        bool success = (matches.str(2).compare("-") != 0);
        return authFromMatches(matches, success);
    }else{
        return {};
    }
}

LogSupervisor::Authentication Apache::authFromMatches(const smatch& matches, bool success){
    Timestamp ts("Not supported yet");
    Address origin = matches.str(1);
    User user = matches.str(2);
    return Authentication(user, origin, ts, success);
}


vector<LogSupervisor::Authentication> Apache::all(){
    return m_auths;
}


} // namespace