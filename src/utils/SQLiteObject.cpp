#include "SQLiteObject.hpp"
using namespace std;

namespace LogSupervisor{

SQLiteStatement::SQLiteStatement(sqlite3* db, sqlite3_stmt* const stmt) :
        preparedStatement(stmt),
        db(db),
        requestFinished(false)
{}

SQLiteStatement::~SQLiteStatement(){
    sqlite3_finalize(preparedStatement);
}

void SQLiteStatement::bindInt(int index, int value){
    int rc = sqlite3_bind_int(preparedStatement, index, value);
    if(rc != SQLITE_OK){
        throw SQLError(to_string(rc) + ": unable to bind " + to_string(value) + " to the " + to_string(index) + "th argument of the statement.");
    }
}

void SQLiteStatement::bindText(int index, const std::string& value){
    int rc = sqlite3_bind_text(preparedStatement, index, value.c_str(), value.size(), SQLITE_TRANSIENT);
    if(rc != SQLITE_OK){
        throw SQLError(to_string(rc) + ": Unable to bind " + value + " to the " + to_string(index) + "th argument of the statement.");
    }
}

int SQLiteStatement::step(){
    if(requestFinished){
        throw SQLError("It is invalid to call step() again on this prepared statement");
    }
    int rc = sqlite3_step(preparedStatement);
    if(rc == SQLITE_DONE){
        // This request has terminated, and step() should not be called again
        requestFinished = true;
    }else if(rc != SQLITE_ROW){
        throw SQLError(string("SQL error: ") + to_string(rc) + " (" + sqlite3_errmsg(db) + ")");
    }
    return rc;
}

int SQLiteStatement::intValue(int iCol){
    return sqlite3_column_int(preparedStatement, iCol);
}
const unsigned char * SQLiteStatement::textValue(int iCol){
    return sqlite3_column_text(preparedStatement, iCol);
}







SQLiteObject::SQLiteObject(const std::string& location) :
    db(NULL)
{
    int rc = sqlite3_open(location.c_str(), &db);
    if(rc != SQLITE_OK){
        throw SQLError("Unable to create an in-memory database");
    }
}
SQLiteObject::~SQLiteObject(){
    if(db){
        sqlite3_close(db);
    }
}

std::shared_ptr<SQLiteStatement> SQLiteObject::prepare(const std::string& statement) const{
    sqlite3_stmt* handle;
    const char* syntaxError;
    int rc = sqlite3_prepare_v2(db, statement.c_str(), statement.size(), &handle, &syntaxError);
    if(rc != SQLITE_OK){
        throw SQLError(string("Invalid statement around '") + syntaxError + "'");
    }
    return make_shared<SQLiteStatement>(db, handle);
}

void SQLiteObject::exec(const string& statement){
    char *errmsg;
    int rc = sqlite3_exec(db, statement.c_str(), NULL, NULL, &errmsg);
    if(rc != SQLITE_OK){
        string what = string("SQL Error: ") + errmsg;
        sqlite3_free(errmsg);
        throw SQLError(what);
    }
}


} // namespace
