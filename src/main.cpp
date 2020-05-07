#include <iostream>
using namespace std;

#include <cxxopts.hpp> // in the libs/ folder

#include "parsers/LogParserInterface.hpp"
#include "parsers/AuthParser.hpp"
#include "utils/Mail.hpp"
#include "Supervisor.hpp"
#include "Config.hpp"
#include "database/Database.hpp"
#include "database/DatabaseObjects.hpp"
#include "serializers/TextSerializer.hpp"
#include "serializers/HTMLSerializer.hpp"
#include "utils/constants.hpp"

using namespace LogSupervisor;

int run(const string& confPath, const string& type, const string& logPath){
    cerr << "conf " << confPath << endl;
    cerr << "type " << type << endl;
    cerr << "log " << logPath << endl;
    return 1;
}


int main(int argc, char* argv[]){
    cxxopts::Options options("access-log-supervisor version " GIT_VERSION_STRING,
                             "This program checks various log files for unexpected logins");
    options.add_options()
        ("h,help", "Print this help")
        ("c,config", "config file", cxxopts::value<string>()->default_value("/etc/access-log-supervisor.yml"))
        ("t,type", "log type", cxxopts::value<string>())
        ("input", "stuff", cxxopts::value<string>())
    ;
    options.parse_positional({"input"});

    string conf, type, input;
    try{
        cxxopts::ParseResult args = options.parse(argc, argv);
        if(args.count("help")){
            cout << options.help() << endl;
            return 0;
        }

        conf = args["config"].as<string>();
        type = args["type"].as<string>();
        input = args["input"].as<string>();
    }catch(const cxxopts::OptionException& e){
        cerr << "Invalid arguments: " << e.what() << endl;
        return 1;
    }catch(const exception& e){
        cerr << "Unable to parse arguments: " << e.what() << endl;
        return 2;
    }
    return run(conf, type, input);
}
