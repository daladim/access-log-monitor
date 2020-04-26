#include "SQLiteDB.hpp"
using namespace std;

namespace SQLite{

DB::DB(const std::string& location) :
    db(NULL)
{
    int rc = sqlite3_open(location.c_str(), &db);
    if(rc != SQLITE_OK){
        throw SQLError("Unable to create an in-memory database");
    }
}
DB::~DB(){
    if(db){
        sqlite3_close(db);
    }
}

std::shared_ptr<Statement> DB::prepare(const std::string& statement) const{
    sqlite3_stmt* handle;
    const char* syntaxError;
    int rc = sqlite3_prepare_v2(db, statement.c_str(), statement.size(), &handle, &syntaxError);
    if(rc != SQLITE_OK){
        throw SQLError(string("Invalid statement around '") + syntaxError + "'");
    }
    return make_shared<Statement>(db, handle);
}

void DB::exec(const string& statement){
    char *errmsg;
    int rc = sqlite3_exec(db, statement.c_str(), NULL, NULL, &errmsg);
    if(rc != SQLITE_OK){
        string what = string("SQL Error: ") + errmsg;
        sqlite3_free(errmsg);
        throw SQLError(what);
    }
}


} // namespace
