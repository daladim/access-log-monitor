#include "catch.hpp"
#include <iostream>
using namespace std;

#include "../src/Supervisor.hpp"
#include "../src/parsers/AuthParser.hpp"
#include "../src/database/Database.hpp"
#include "../src/database/DatabaseObjects.hpp"

using namespace LogSupervisor;

// This is mainly an integration test, it does not really classify as a unit test
TEST_CASE( "Main logic for auth.log" ){
    Config conf("doc/config-example.yml");
    LogParser::Interface* logParser = new LogParser::Auth("tests/data/auth.log");
    Supervisor sup(conf, logParser);

    sup.run();

    const Database::Database& db = sup.database();
    int i = 0;
    for(auto line : db.all()){
        switch(i){
            case 0:
                CHECK( line->user.compare("john") == 0 );
                CHECK( line->origin == Address("2001:2222:2222:b10c::99") );
                // This auth matches a specific rule
                CHECK( line->validity == Authentication::Validity::OK() );
                CHECK( line->description.compare("regular access") == 0 );
            break;

            case 1:
                CHECK( line->user.compare("john") == 0 );
                CHECK( line->origin == Address("66.66.66.66") );
                // This auth has the default status
                CHECK( line->validity == Authentication::Validity::Critical() );
            break;

            case 2:
                CHECK( line->user.compare("ringo") == 0 );
                CHECK( line->origin == Address("127.0.0.1") );
                // This auth matches a specific rule
                CHECK( line->validity == Authentication::Validity::Warning() );
                CHECK( line->description.compare("this rules does not use any aliases") == 0 );
            break;

            case 3:
                CHECK( line->user.compare("dtrump") == 0 );
                CHECK( line->origin == Address("1.2.3.4") );
                // This auth matches a wildcard rule
                CHECK( line->validity == Authentication::Validity::Warning() );
            break;

        }
        ++i;
    }

    delete logParser;
}

