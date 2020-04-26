#include "catch.hpp"
#include <iostream>
#include <cstring>
using namespace std;

#include "../src/utils/sqlite/SQLiteDB.hpp"
#include "../src/utils/sqlite/SQLiteStatement.hpp"

TEST_CASE( "SQLite wrapper" ){
    // Normal usage
    SQLite::DB d(":memory:");
    string command = "CREATE TABLE companies(names TEXT, age INT, address CHAR(50))";
    string simpleInsertion = "INSERT INTO companies (names, age, address) "\
                             "VALUES ('ACME', 100, '1 Main Street, 1111 Megapolis');";
    string preparedInsertion = "INSERT INTO companies (names, age, address) "\
                               "VALUES (?, ?, ?);";

    REQUIRE_NOTHROW( d.exec(command) );
    REQUIRE_NOTHROW( d.exec(simpleInsertion) );

    string longField = "National Megastuff, Inc. (previously Shenzhen Sprockets and Tools, Co. Ltd.)";
    shared_ptr<SQLite::Statement> write = d.prepare(preparedInsertion);
    write->bindInt(2, 10);
    write->bindText(1, longField);
    write->bindText(3, "Venus");
    REQUIRE_NOTHROW( write->step() );

    shared_ptr<SQLite::Statement> read = d.prepare("SELECT * FROM companies;");
    REQUIRE_THROWS_AS( read->textValue(0) , std::logic_error /* because step() has not been called already */ );
    read->step();
    REQUIRE( strncmp( "ACME", (const char*)read->textValue(0), 5) == 0 );
    REQUIRE( 100 == read->intValue(1) );

    read->step();
    REQUIRE( longField.compare((const char*)read->textValue(0)   ) == 0 );
    REQUIRE( strncmp( "Venus", (const char*)read->textValue(2), 5) == 0 );
    REQUIRE( 10 == read->intValue(1) );



    // Invalid usage
    REQUIRE_THROWS_AS( d.exec("This statement does not mean anything") , SQLite::SQLError );

    REQUIRE_THROWS_AS( d.exec("INSERT INTO COMAPANY (NAME) VALUES ('acme, but no ID is given although it is NOT NULL');") , SQLite::SQLError );
}
