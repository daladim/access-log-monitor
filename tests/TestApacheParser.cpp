#include "catch.hpp"
#include <iostream>
using namespace std;

#include "../src/ApacheParser.hpp"
using namespace LogSupervisor;

TEST_CASE( "Parsing SSL combined log files" ){
    LogParser::Apache apacheParser("tests/data/ssl_access.log");
    apacheParser.parseLog();

    vector<Authentication> auths = apacheParser.all();
    REQUIRE( auths.size() == 14 );

    Authentication a0 = auths[0];
    CHECK( a0.user.compare("-") == 0 );
    CHECK( a0.success == false );
    CHECK( a0.origin == Address("123.45.67.89") );

    Authentication a3 = auths[3];
    CHECK( a3.user.compare("johnson") == 0 );
    CHECK( a3.success == true );
    CHECK( a3.origin == Address("99.88.77.66") );

    Authentication a9 = auths[9];
    CHECK( a9.user.compare("-") == 0 );
    CHECK( a9.success == false );
    CHECK( a9.origin == Address("::1") );

    Authentication a14 = auths[13];
    CHECK( a14.user.compare("jackson") == 0 );
    CHECK( a14.success == true );
    CHECK( a14.origin == Address("::1") );

}

