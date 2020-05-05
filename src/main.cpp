#include <iostream>
using namespace std;

#include "utils/LogParserInterface.hpp"
#include "Config.hpp"
#include "AuthParser.hpp"
#include "database/Database.hpp"
#include "database/DatabaseObjects.hpp"
#include "Supervisor.hpp"
#include "TextSerializer.hpp"
#include "HTMLSerializer.hpp"

using namespace LogSupervisor;


int main(){
    cerr << "running..." << endl;

    Config conf("doc/config-example.yml");
    LogParser::Interface* logParser = new LogParser::Auth("tests/data/auth.log");
    Supervisor sup(conf, logParser);
    sup.run();

    Serializer::Interface* serializer1 = new Serializer::Text(sup.database());
    Serializer::Interface* serializer2 = new Serializer::HTML(sup.database());
    cout << *serializer1 << endl;
    cout << *serializer2 << endl;

    return 0;
}