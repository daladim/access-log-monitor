#include <iostream>
using namespace std;

#include "DatabaseRequest.hpp"

namespace LogSupervisor::Database{

Request::Request(std::shared_ptr<SQLite::Statement> statement) :
    statement(statement)
{}

Request::Iterator Request::begin(){
    return Request::Iterator(statement->begin());
}

Request::Iterator Request::end(){
    return Request::Iterator(statement->end());
}

shared_ptr<Authentication> Request::firstRow(){
    Request::Iterator it = statement->begin();
    return *it;
}



} // namespace