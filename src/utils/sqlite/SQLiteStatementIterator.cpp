#include "SQLiteStatementIterator.hpp"
using namespace std;

namespace SQLite{

StatementIterator::StatementIterator(SQLite::Statement& statement, bool thisIsAnEndIteratorOnly) :
    statement(statement),
    thisIsAnEndIteratorOnly(thisIsAnEndIteratorOnly)
{
    if(statement.hasStarted() == false){
        statement.step();
    }
}

const SQLite::Statement& StatementIterator::operator*(){
    return statement;
}

const SQLite::Statement* StatementIterator::operator->(){
    return &statement;
}

SQLite::Statement& StatementIterator::operator++(){
    statement.step(); // this may throw in case the caller iterates too much and has not checked the end of the iteration
    return statement;
}

bool StatementIterator::isAtEnd() const{
    if(thisIsAnEndIteratorOnly){
        return true;
    }
    return statement.isExhausted();
}

bool StatementIterator::operator==(const StatementIterator& other) const{
    return(   addressof(statement) == addressof(other.statement)
           && isAtEnd()
           && other.isAtEnd()
          );
}

bool StatementIterator::operator!=(const StatementIterator& other) const{
    return !(*this == other);
}




} //namespace