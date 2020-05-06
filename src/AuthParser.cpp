#include "AuthParser.hpp"

#include <string>
using namespace std;

namespace LogSupervisor::LogParser{

Auth::Auth(const string& filePath) :
    logFile(filePath)
{}

std::string Auth::humanReadableLogType() const{
    return "SSH log checks";
}


void Auth::parseLog(){
    string currentLine;
    while(std::getline(logFile, currentLine)){
        optional<Authentication> auth = parseLine(currentLine);
        if(auth){
            m_auths.push_back(*auth);
        }
    }

}

optional<LogSupervisor::Authentication> Auth::parseLine(const std::string& line){
    //                          TS     host   proc   PID                                user              IP
    regex re_success_pk(R"raw(^(.*)\s+[^\s]+\s+.*\[[0-9]+\]: Accepted publickey for\s+([^\s]+)\s+from\s+([^\s]+)\s.*)raw");
    regex re_success_pw(R"raw(^(.*)\s+[^\s]+\s+.*\[[0-9]+\]: Accepted password for\s+([^\s]+)\s+from\s+([^\s]+)\s.*)raw");

    //                          TS     host   proc   PID                                          user              IP
    regex re_failed(    R"raw(^(.*)\s+[^\s]+\s+.*\[[0-9]+\]: Failed password for invalid user\s+([^\s]+)\s+from\s+([^\s]+)\s.*)raw");

    smatch matches;
    if(regex_search(line, matches, re_success_pk)){
        return authFromMatches(matches, true);
    }else if(regex_search(line, matches, re_success_pw)){
        return authFromMatches(matches, true);
    }else if(regex_search(line, matches, re_failed)){
        return authFromMatches(matches, false);
    }else{
        return {};
    }
}

LogSupervisor::Authentication Auth::authFromMatches(const smatch& matches, bool success){
    Timestamp ts = matches.str(1);
    User user = matches.str(2);
    Address origin = matches.str(3);
    return Authentication(user, origin, ts, success);
}


vector<LogSupervisor::Authentication> Auth::all(){
    return m_auths;
}


} // namespace