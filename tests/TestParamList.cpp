#include "catch.hpp"
#include <iostream>
#include <vector>
using namespace std;

#include "../src/utils/ParamList.hpp"
#include "../src/utils/Address.hpp"
#include "../src/utils/AddressRange.hpp"
using namespace LogSupervisor;

TEST_CASE( "Param lists with string" ){

    SECTION("Wildcard"){
        ParamList<string, string> ps;
        CHECK( ps.contains("anything at all") == true );
    }

    SECTION("Constructed with a single param"){
        ParamList<string, string> ps(vector<string>({"abc"}));
        CHECK( ps.contains("abc") == true );
        CHECK( ps.contains("def") == false );
    }

    SECTION("Inserted a single param"){
        ParamList<string, string> ps;
        ps.insert(vector<string>({"abc"}));
        CHECK( ps.contains("abc") == true );
        CHECK( ps.contains("def") == false );
    }

    SECTION("Constructed with multiple params"){
        ParamList<string, string> ps(vector<string>({"123", "456"}));
        CHECK( ps.contains("123") == true );
        CHECK( ps.contains("456") == true );
        CHECK( ps.contains("789") == false );
    }

    SECTION("Inserted multiple params"){
        ParamList<string, string> ps;
        ps.insert(vector<string>({"123", "456"}));
        CHECK( ps.contains("123") == true );
        CHECK( ps.contains("456") == true );
        CHECK( ps.contains("789") == false );
    }

}

TEST_CASE( "Param lists with AddressRange" ){
    SECTION("Wildcard"){
        ParamList<AddressRange, Address> ps;
        CHECK( ps.contains("1.2.3.4") == true );
    }

    SECTION("Constructed with a single param"){
        ParamList<AddressRange, Address> ps(vector<AddressRange>({"10.0.0.0/8"}));
        CHECK( ps.contains("10.11.12.13") == true );
        CHECK( ps.contains("1.2.3.4") == false );
    }

    SECTION("Inserted a single param"){
        ParamList<AddressRange, Address> ps;
        ps.insert(vector<AddressRange>({"10.0.0.0/8"}));
        CHECK( ps.contains("10.11.12.13") == true );
        CHECK( ps.contains("1.2.3.4") == false );
    }

    SECTION("Constructed with multiple params"){
        ParamList<AddressRange, Address> ps(vector<AddressRange>({"10.0.0.0/8", "192.168.0.0/24"}));
        CHECK( ps.contains("10.11.12.13") == true );
        CHECK( ps.contains("192.168.0.10") == true );
        CHECK( ps.contains("1.2.3.4") == false );
    }

    SECTION("Inserted multiple params"){
        ParamList<AddressRange, Address> ps;
        ps.insert(vector<AddressRange>({"10.0.0.0/8", "192.168.0.0/24"}));
        CHECK( ps.contains("10.11.12.13") == true );
        CHECK( ps.contains("192.168.0.10") == true );
        CHECK( ps.contains("1.2.3.4") == false );
    }
}
