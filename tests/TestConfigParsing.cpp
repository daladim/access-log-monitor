#include "catch.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h> // in the libs/ folder
using namespace std;

#include "../src/Config.hpp"
using namespace LogSupervisor;

// This is an integration test, not a unit test
TEST_CASE( "Config file parsing" ){
    Config c("doc/config-example.yml");

    CHECK( c.default_status() == Authentication::Validity::Critical() );

    const vector<Rule> rules = c.rules();
    SECTION("A completely defined rule"){
        const Rule& r0 = rules[0];
        CHECK( r0.users.contains("Me") == false );
        CHECK( r0.users.contains("john") == true );
        CHECK( r0.addresses.contains("10.1.2.3") == true );
        CHECK( r0.addresses.contains("111.222.111.222") == true );
        CHECK( r0.validity == Authentication::Validity::OK() );
        CHECK( r0.descr.compare("regular access") == 0 );
    }

    SECTION("A rule with wildcards"){
        const Rule& r3 = rules[3];
        CHECK( r3.users.contains("A user that was never defined") == true );
        CHECK( r3.addresses.contains("1.2.3.1") == true );
        CHECK( r3.addresses.contains("2.2.2.2") == false );
        CHECK( r3.validity == Authentication::Validity::Warning() );
        CHECK( r3.descr.compare("I have a specific interest on these addresses") == 0 );
    }

}

TEST_CASE( "Invalid config file" ){
    // The Doxyfile is not a YAML file
    string wrongConfigFile("doc/Doxyfile");
    cerr << "Note: this test checks for an expected exception. It will display an error about " << wrongConfigFile << ", that you can (and should) safely ignore, as long as the test framework reports that all tests are passing." << endl;
    CHECK_THROWS_AS( Config(wrongConfigFile), YAML::Exception );
}
