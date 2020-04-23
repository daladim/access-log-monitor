#include "SQLiteStatement.hpp"
#include "SQLiteStatementIterator.hpp"
using namespace std;

namespace SQLite{

Statement::Statement(sqlite3* db, sqlite3_stmt* const stmt) :
        preparedStatement(stmt),
        db(db),
        hasStepped(false),
        requestFinished(false)
{}

Statement::~Statement(){
    sqlite3_finalize(preparedStatement);
}

void Statement::bindInt(int index, int value){
    int rc = sqlite3_bind_int(preparedStatement, index, value);
    if(rc != SQLITE_OK){
        throw SQLError(to_string(rc) + ": unable to bind " + to_string(value) + " to the " + to_string(index) + "th argument of the statement.");
    }
}

void Statement::bindText(int index, const std::string& value){
    int rc = sqlite3_bind_text(preparedStatement, index, value.c_str(), value.size(), SQLITE_TRANSIENT);
    if(rc != SQLITE_OK){
        throw SQLError(to_string(rc) + ": Unable to bind " + value + " to the " + to_string(index) + "th argument of the statement.");
    }
}

int Statement::step(){
    if(requestFinished){
        throw logic_error("It is invalid to call step() again on this prepared statement");
    }
    int rc = sqlite3_step(preparedStatement);
    hasStepped = true;
    if(rc == SQLITE_DONE){
        // This request has terminated, and step() should not be called again
        requestFinished = true;
    }else if(rc != SQLITE_ROW){
        throw SQLError(string("SQL error: ") + to_string(rc) + " (" + sqlite3_errmsg(db) + ")");
    }
    return rc;
}

int Statement::intValue(int iCol) const{
    if(hasStepped == false){
        throw logic_error("step() must be called first");
    }
    if(requestFinished){
        throw logic_error("You are past the last row for this statement");
    }
    return sqlite3_column_int(preparedStatement, iCol);
}
const unsigned char * Statement::textValue(int iCol) const{
    if(hasStepped == false){
        throw logic_error("step() must be called first");
    }
    if(requestFinished){
        throw logic_error("You are past the last row for this statement");
    }
    return sqlite3_column_text(preparedStatement, iCol);
}



SQLite::StatementIterator Statement::begin(){
    return StatementIterator(*this);
}
const SQLite::StatementIterator Statement::end(){
    return StatementIterator(*this, true);
}

} // namespace
