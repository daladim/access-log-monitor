#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/utils/AddressRange.hpp"
#include "../src/utils/Address.hpp"
using namespace std;
using namespace LogSupervisor;


TEST_CASE( "IPv4 single addresses" ){
    REQUIRE_THROWS( Address("10.0.0.0/8") );

    const char str_addrOne[] = "192.168.0.1";
    const char str_rangeOne[] = "192.168.0.1/10";
    Address addr(str_addrOne);
    REQUIRE_THROWS_AS( addr.contains(Address(str_addrOne)) , std::logic_error );
    REQUIRE_THROWS_AS( addr.contains(AddressRange(str_rangeOne)) , std::logic_error );
}


TEST_CASE( "IPv4 address ranges" ) {
    const char str_range[] = "10.0.0.0/8";
    AddressRange range(str_range);

    REQUIRE( range.contains(Address("10.20.30.40")) == true );
    REQUIRE( range.contains(Address("1.2.3.4")) == false );

    REQUIRE( range.to_string()->compare(str_range) == 0 );

}



TEST_CASE( "IPv6 single addresses" ){


}


TEST_CASE( "IPv6 address ranges" ) {

}