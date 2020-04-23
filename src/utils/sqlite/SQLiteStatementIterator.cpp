#include "SQLiteStatementIterator.hpp"
using namespace std;

namespace SQLite{

StatementIterator::StatementIterator(std::shared_ptr<SQLite::Statement> statement) :
    statement(statement)
{
    if(statement->hasStarted() == false){
        statement->step();
    }
}

const SQLite::Statement& StatementIterator::operator*(){
    return *statement;
}

const SQLite::Statement* StatementIterator::operator->(){
    return statement.get();
}

SQLite::Statement& StatementIterator::operator++(){
    statement->step(); // this may throw in case the caller iterates too much and has not checked the end of the iteration
    return *statement;
}




} //namespace