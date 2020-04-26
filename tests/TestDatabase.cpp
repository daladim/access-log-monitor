#include "catch.hpp"
#include <iostream>
using namespace std;

#include "../src/utils/Database.hpp"
using namespace LogSupervisor;

TEST_CASE( "Database" ){
    // Normal usage
    Database d;
    d.insert(Authentication("Joe",     "1.1.1.1", "2000-01-01 11:22:33"));
    d.insert(Authentication("Jack",    "2.2.2.2", "2010-01-01 11:22:33"));
    d.insert(Authentication("William", "3.3.3.3", "2020-01-01 11:22:33"));
    d.insert(Authentication("Averell", "4.4.4.4", "2030-01-01 11:22:33"));

    vector<shared_ptr<const Authentication>> items = d.all();
    REQUIRE( items.size() == 4 );

    // Invalid usage

}

