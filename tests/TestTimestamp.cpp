#include "catch.hpp"
#include <iostream>
#include <cstring>
using namespace std;

#include "../src/utils/Timestamp.hpp"
using namespace LogSupervisor;

TEST_CASE( "Timestamp" ){
    const char* stime = "2000-01-01 10:00:00";
    Timestamp t = stime;
    REQUIRE( t.to_string().compare(stime) == 0 );
}

