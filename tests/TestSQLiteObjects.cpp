#include "catch.hpp"
#include <iostream>
#include <cstring>
using namespace std;

#include "../src/utils/sqlite/SQLiteDB.hpp"
#include "../src/utils/sqlite/SQLiteStatement.hpp"
#include "../src/utils/sqlite/SQLiteStatementIterator.hpp"

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

TEST_CASE( "Iterator for SQL queries" ){
    SQLite::DB d(":memory:");
    d.exec("CREATE TABLE dwarfs(name TEXT, ord INT)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Doc', 1)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Dopey', 2)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Bashful', 3)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Grumpy', 4)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Sneeze', 5)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Sleepy', 6)");
    d.exec("INSERT INTO dwarfs (name, ord) VALUES ('Happy', 7)");

    shared_ptr<SQLite::Statement> read = d.prepare("SELECT * FROM dwarfs;");
    SQLite::StatementIterator iter(read);

    SECTION("Manually incrementing a StatementIterator"){
        // A StatementIterator runs the first step() automatically
        REQUIRE( iter->intValue(1) == 1 );
        REQUIRE( (*iter).intValue(1) == 1 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Doc", 4) == 0 );
        REQUIRE( iter->intValue(1) == 1 );
        REQUIRE( iter->intValue(1) == 1 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Doc", 4) == 0 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Doc", 4) == 0 );

        ++iter;
        REQUIRE( iter->intValue(1) == 2 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Dopey", 4) == 0 );
        REQUIRE( iter->intValue(1) == 2 );
        REQUIRE( iter->intValue(1) == 2 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Dopey", 4) == 0 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Dopey", 4) == 0 );

        ++iter;
        ++iter;
        ++iter;
        REQUIRE( iter->intValue(1) == 5 );
        REQUIRE( strncmp( (const char*)iter->textValue(0), "Sneeze", 4) == 0 );

        ++iter;
        ++iter;
        REQUIRE( iter->intValue(1) == 7 );
        ++iter;
        // We are now past the last row
        REQUIRE_THROWS_AS( iter->intValue(1) , logic_error );
        REQUIRE_THROWS_AS( ++iter, logic_error );
    }
}
