#include "catch.hpp"
#include <iostream>
using namespace std;

#include "../src/AuthParser.hpp"
using namespace LogSupervisor;

TEST_CASE( "Parsing auth.log files" ){
    LogParser::Auth authParser("tests/data/auth.log");
    authParser.parseLog();

    vector<Authentication> auths = authParser.all();
    REQUIRE( auths.size() == 4 );

    Authentication a0 = auths[0];
    CHECK( a0.user.compare("john") == 0 );
    CHECK( a0.success == true );
    CHECK( a0.origin == Address("2001:2222:2222:b10c::99") );

    Authentication a1 = auths[1];
    CHECK( a1.user.compare("john") == 0 );
    CHECK( a1.success == true );
    CHECK( a1.origin == Address("66.66.66.66") );

    Authentication a2 = auths[2];
    CHECK( a2.user.compare("ringo") == 0 );
    CHECK( a2.success == false );


}

