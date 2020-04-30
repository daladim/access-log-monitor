#include <iostream>
#include <fstream>
using namespace std;

#include "Supervisor.hpp"
#include "utils/Rule.hpp"

namespace LogSupervisor{

Supervisor::Supervisor(const Config& config, LogParser::Interface* logParser) :
    config(config),
    logParser(logParser)
{}

void Supervisor::run(){
    // Get the user-defined rules
    const std::vector<Rule>& rules = config.rules();

    // Parse the log
    logParser->parseLog();

    // Check every detected authentication
    vector<Authentication> auths = logParser->all();
    for(Authentication& auth : auths){
        const User& user = auth.user;
        const Address& origin = auth.origin;

        // Store default values...
        auth.validity = config.default_status();
        auth.description = "";

        // ...unless a rule applies
        for(const Rule& rule : rules){
            if(rule.matches(user, origin)){
                auth.validity = rule.validity;
                auth.description = rule.descr;
                break;
            }
        }

        db.insert(auth);
    }
}




} // namespace

