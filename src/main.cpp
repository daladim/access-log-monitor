#include <iostream>
using namespace std;

#include <cxxopts.hpp> // in the libs/ folder

#include "parsers/LogParserInterface.hpp"
#include "parsers/AuthParser.hpp"
#include "parsers/ApacheParser.hpp"
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
    Config conf(confPath);

    LogParser::Interface* parser;
    if(type.compare(LogParser::Apache::shortName()) == 0){
        parser = new LogParser::Apache(logPath);
    }else if(type.compare(LogParser::Auth::shortName()) == 0){
        parser = new LogParser::Auth(logPath);
    }else{
        throw runtime_error(string("Invalid type value: ") + type);
    }

    Supervisor sup(conf, parser);
    sup.run();

    Serializer::HTML serializer(sup.database());

    // Send a mail if needed
    if(conf.mailAddress()){
        string title = string("Supervision, ") + parser->humanReadableLogType();
        Mail mail(*(conf.mailAddress()), title, serializer.to_string(), logPath);
        bool res =mail.send();
        if(res == false){
            cerr << "Something wicked happened when sending the mail. It may not have been sent." << endl;
            // At least print it to stdout
            cout << serializer.to_string() << endl;
        }
    }else{
        // Otherwise, only display to stdout
        cout << serializer.to_string() << endl;
    }

    delete parser;
    return 0;
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


    // Parse the arguments...
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
        cerr << endl;
        cerr << options.help() << endl;
        return 2;
    }


    // ...and actually run the checker
    try{
        run(conf, type, input);
    }catch(const exception& e){
        cerr << "Error: " << e.what() << endl;
        return 10;
    }
    return 0;
}
