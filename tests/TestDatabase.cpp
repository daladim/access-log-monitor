#include "catch.hpp"
#include <iostream>
using namespace std;

#include "../src/database/Database.hpp"
using namespace LogSupervisor;

void check_cpp_implem(Database::Request& req){
    SECTION("Manually incrementing Request::Iterator"){
        Database::Request::Iterator it = req.begin();
        CHECK( it->user.compare( "Joe" ) == 0 );
        CHECK( it->origin.to_string()->rfind("1.1.1.1") != string::npos );

        ++it;
        CHECK( it->user.compare( "Jack" ) == 0 );
        CHECK( it->origin.to_string()->rfind("2.2.2.2") != string::npos );

        ++it;
        CHECK( it->user.compare( "William" ) == 0 );
        CHECK( it->origin.to_string()->rfind("3.3.3.3") != string::npos );

        ++it;
        CHECK( it->user.compare( "Averell" ) == 0 );
        CHECK( it->origin.to_string()->rfind("4.4.4.4") != string::npos );
        CHECK( it != req.end() );

        ++it; // We are now at the end
        CHECK( it == req.end() );
        CHECK_THROWS_AS( ++it, logic_error );
    }


    SECTION("Using request.begin() and end()"){
        Database::Request::Iterator end = req.end();
        int i = 0;
        for(Database::Request::Iterator it = req.begin(); it != end ; ++it){
            switch(i){
                case 0: CHECK( it->user.compare( "Joe"     ) == 0 ); break;
                case 1: CHECK( it->user.compare( "Jack"    ) == 0 ); break;
                case 2: CHECK( it->user.compare( "William" ) == 0 ); break;
                case 3: CHECK( it->user.compare( "Averell" ) == 0 ); break;
            }
            ++i;
        }
    }

    SECTION("Range-based loops for requests"){
        int i = 0;
        for(shared_ptr<Authentication> auth : req){
            switch(i){
                case 0: CHECK( auth->user.compare( "Joe"     ) == 0 ); break;
                case 1: CHECK( auth->user.compare( "Jack"    ) == 0 ); break;
                case 2: CHECK( auth->user.compare( "William" ) == 0 ); break;
                case 3: CHECK( auth->user.compare( "Averell" ) == 0 ); break;
            }
            ++i;
        }
    }
}


TEST_CASE( "Database" ){
    // Normal usage
    Database::Database d;
    d.insert(Authentication("Joe",     "1.1.1.1", "2000-01-01 11:22:33", true));
    d.insert(Authentication("Jack",    "2.2.2.2", "2010-01-01 11:22:33", false));
    d.insert(Authentication("William", "3.3.3.3", "2020-01-01 11:22:33", false));
    d.insert(Authentication("Averell", "4.4.4.4", "2030-01-01 11:22:33", false));


    SECTION("C++ implementation of requests"){
        Database::Request req = d.all();
        check_cpp_implem(req);
    }


    SECTION("Fetching a row"){
        shared_ptr<Authentication> row1 = d.fetch(1);
        CHECK( row1->user.compare("Joe") == 0 );
        CHECK( row1->success == true );
        CHECK( row1->validity == Authentication::Validity::Undefined );
    }

    SECTION("Fetching another row"){
        shared_ptr<Authentication> row2 = d.fetch(2);
        CHECK( row2->user.compare("Jack") == 0 );
        CHECK( row2->success == false );
        CHECK( row2->validity == Authentication::Validity::Undefined );
    }

    SECTION("Updating a field"){
        d.updateValidity(2, Authentication::Validity::OK );
        shared_ptr<Authentication> row2 = d.fetch(2);
        CHECK( row2->user.compare("Jack") == 0 );
        CHECK( row2->validity == Authentication::Validity::OK );

    }


}

