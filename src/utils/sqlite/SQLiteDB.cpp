#include "SQLiteDB.hpp"
using namespace std;

namespace SQLite{

SQLiteDB::SQLiteDB(const std::string& location) :
    db(NULL)
{
    int rc = sqlite3_open(location.c_str(), &db);
    if(rc != SQLITE_OK){
        throw SQLError("Unable to create an in-memory database");
    }
}
SQLiteDB::~SQLiteDB(){
    if(db){
        sqlite3_close(db);
    }
}

std::shared_ptr<SQLiteStatement> SQLiteDB::prepare(const std::string& statement) const{
    sqlite3_stmt* handle;
    const char* syntaxError;
    int rc = sqlite3_prepare_v2(db, statement.c_str(), statement.size(), &handle, &syntaxError);
    if(rc != SQLITE_OK){
        throw SQLError(string("Invalid statement around '") + syntaxError + "'");
    }
    return make_shared<SQLiteStatement>(db, handle);
}

void SQLiteDB::exec(const string& statement){
    char *errmsg;
    int rc = sqlite3_exec(db, statement.c_str(), NULL, NULL, &errmsg);
    if(rc != SQLITE_OK){
        string what = string("SQL Error: ") + errmsg;
        sqlite3_free(errmsg);
        throw SQLError(what);
    }
}


} // namespace
