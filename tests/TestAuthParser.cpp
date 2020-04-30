#include "catch.hpp"
#include <iostream>
using namespace std;

#include "../src/AuthParser.hpp"
using namespace LogSupervisor;

TEST_CASE( "Parsing auth.log files" ){
    LogParser::Auth authParser("tests/data/auth.log");
    authParser.parseLog();

    vector<Authentication> auths = authParser.all();
    REQUIRE( auths.size() == 3 );

    Authentication a0 = auths[0];
    CHECK( a0.user.compare("dtrump") == 0 );
    CHECK( a0.success == true );
    CHECK( a0.origin == Address("127.0.0.1") );

    Authentication a1 = auths[1];
    CHECK( a1.user.compare("dtrump") == 0 );
    CHECK( a1.success == true );
    CHECK( a1.origin == Address("10.20.30.40") );

    Authentication a2 = auths[2];
    CHECK( a2.user.compare("bobama") == 0 );
    CHECK( a2.success == false );


}

