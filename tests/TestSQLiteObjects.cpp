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

    CHECK_NOTHROW( d.exec(command) );
    CHECK_NOTHROW( d.exec(simpleInsertion) );

    string longField = "National Megastuff, Inc. (previously Shenzhen Sprockets and Tools, Co. Ltd.)";
    shared_ptr<SQLite::Statement> write = d.prepare(preparedInsertion);
    write->bindInt(2, 10);
    write->bindText(1, longField);
    write->bindText(3, "Venus");
    CHECK_NOTHROW( write->step() );

    shared_ptr<SQLite::Statement> read = d.prepare("SELECT * FROM companies;");
    CHECK_THROWS_AS( read->textValue(0) , std::logic_error /* because step() has not been called already */ );
    read->step();
    CHECK( strncmp( "ACME", (const char*)read->textValue(0), 5) == 0 );
    CHECK( 100 == read->intValue(1) );

    read->step();
    CHECK( longField.compare((const char*)read->textValue(0)   ) == 0 );
    CHECK( strncmp( "Venus", (const char*)read->textValue(2), 5) == 0 );
    CHECK( 10 == read->intValue(1) );



    // Invalid usage
    CHECK_THROWS_AS( d.exec("This statement does not mean anything") , SQLite::SQLError );

    CHECK_THROWS_AS( d.exec("INSERT INTO COMAPANY (NAME) VALUES ('acme, but no ID is given although it is NOT NULL');") , SQLite::SQLError );
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

    SECTION("Manually incrementing a StatementIterator"){
        SQLite::StatementIterator iter(*read);

        // A StatementIterator runs the first step() automatically
        CHECK( iter->intValue(1) == 1 );
        CHECK( (*iter).intValue(1) == 1 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Doc", 4) == 0 );
        CHECK( iter->intValue(1) == 1 );
        CHECK( iter->intValue(1) == 1 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Doc", 4) == 0 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Doc", 4) == 0 );

        ++iter;
        CHECK( iter->intValue(1) == 2 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Dopey", 4) == 0 );
        CHECK( iter->intValue(1) == 2 );
        CHECK( iter->intValue(1) == 2 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Dopey", 4) == 0 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Dopey", 4) == 0 );

        ++iter;
        ++iter;
        ++iter;
        CHECK( iter->intValue(1) == 5 );
        CHECK( strncmp( (const char*)iter->textValue(0), "Sneeze", 4) == 0 );

        ++iter;
        ++iter;
        CHECK( iter->intValue(1) == 7 );
        ++iter;
        // We are now past the last row
        CHECK_THROWS_AS( iter->intValue(1) , logic_error );
        CHECK_THROWS_AS( ++iter, logic_error );
    }

    SECTION("Using begin() and end() in a loop"){
        auto endIterator = read->end();
        int i = 1;
        for(auto it = read->begin(); it != endIterator; ++it){
            switch(i){
                case 1: CHECK( strncmp( (const char*)it->textValue(0), "Doc", 4) == 0); break;
                case 2: CHECK( strncmp( (const char*)it->textValue(0), "Dopey", 6) == 0); break;
                case 3: CHECK( strncmp( (const char*)it->textValue(0), "Bashful", 8) == 0); break;
                case 4: CHECK( strncmp( (const char*)it->textValue(0), "Grumpy", 7) == 0); break;
                case 5: CHECK( strncmp( (const char*)it->textValue(0), "Sneeze", 7) == 0); break;
                case 6: CHECK( strncmp( (const char*)it->textValue(0), "Sleepy", 7) == 0); break;
                case 7: CHECK( strncmp( (const char*)it->textValue(0), "Happy", 6) == 0); break;
            }
            ++i;
        }
    }

    // Range-based loops require SQLite::Statement to be copiable
    //    for(auto it : *read){
    //        cerr << "item" << endl;
    //    }
}
