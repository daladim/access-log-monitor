#include "DatabaseRequest.hpp"
using namespace std;


namespace LogSupervisor::Database{


Request::Iterator::Iterator(SQLite::Statement::Iterator sqlsi) :
    sqlsi(sqlsi)
{}

std::shared_ptr<LogSupervisor::Authentication> Request::Iterator::currentAuth(){
    int id =                       sqlsi->intValue(0);
    User user =      (const char*) sqlsi->textValue(1);
    Address origin = (const char*) sqlsi->textValue(2);
    Timestamp ts =   (const char*) sqlsi->textValue(3);
    return make_shared<LogSupervisor::Authentication>(user, origin, ts, id);
}

std::shared_ptr<LogSupervisor::Authentication> Request::Iterator::operator++(){
    ++sqlsi;
    if(sqlsi->isExhausted()){
        return nullptr; /* Since the iterator is equal to end(), it should not be dereferenced anyway */
    }else{
        return currentAuth();
    }
}
const std::shared_ptr<LogSupervisor::Authentication> Request::Iterator::operator*(){
    return currentAuth();
}
const std::shared_ptr<LogSupervisor::Authentication> Request::Iterator::operator->(){
    return currentAuth();
}
bool Request::Iterator::operator==(const Request::Iterator& other) const{
    return(sqlsi == other.sqlsi);
}
bool Request::Iterator::operator!=(const Request::Iterator& other) const{
    return(sqlsi != other.sqlsi);
}

} // namespace