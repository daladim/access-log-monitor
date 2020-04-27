#include "SQLiteStatement.hpp"
using namespace std;

namespace SQLite{

Statement::Iterator::Iterator(SQLite::Statement& statement, bool thisIsAnEndIteratorOnly) :
    statement(statement),
    thisIsAnEndIteratorOnly(thisIsAnEndIteratorOnly)
{
    if(statement.hasStarted() == false){
        statement.step();
    }
}

const SQLite::Statement& Statement::Iterator::operator*(){
    return statement;
}

const SQLite::Statement* Statement::Iterator::operator->(){
    return &statement;
}

SQLite::Statement& Statement::Iterator::operator++(){
    statement.step(); // this may throw in case the caller iterates too much and has not checked the end of the iteration
    return statement;
}

bool Statement::Iterator::isAtEnd() const{
    if(thisIsAnEndIteratorOnly){
        return true;
    }
    return statement.isExhausted();
}

bool Statement::Iterator::operator==(const Iterator& other) const{
    return(   addressof(statement) == addressof(other.statement)
           && isAtEnd()
           && other.isAtEnd()
          );
}

bool Statement::Iterator::operator!=(const Iterator& other) const{
    return !(*this == other);
}




} //namespace