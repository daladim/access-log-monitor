#ifndef _SQLITE_OBJECT_HPP_
#define _SQLITE_OBJECT_HPP_

#include <sqlite3.h>
#include <memory>

namespace LogSupervisor{

class SQLError : public std::runtime_error{
public:
    SQLError(const string& what) : std::runtime_error(what) {}
};

//! A wrapper around sqlite3_stmt* ojects
class SQLiteStatement{
public:
    SQLiteStatement(sqlite3* db, sqlite3_stmt* const stmt) :
        preparedStatement(stmt),
        db(db),
        requestFinished(false)
    {}
    SQLiteStatement(const SQLiteStatement&) = delete;
    SQLiteStatement& operator=(const SQLiteStatement& rhs) = delete;
    ~SQLiteStatement(){
        sqlite3_finalize(preparedStatement);
    }

    //! Bind a user-controlled int input to the statement
    void bindInt(int index, int value){
        int rc = sqlite3_bind_int(preparedStatement, index, value);
        if(rc != SQLITE_OK){
            throw SQLError(to_string(rc) + ": unable to bind " + to_string(value) + " to the " + to_string(index) + "th argument of the statement.");
        }
    }

    //! Bind a user-controlled text input to the statement
    void bindText(int index, const std::string& value){
        int rc = sqlite3_bind_text(preparedStatement, index, value.c_str(), value.size(), SQLITE_TRANSIENT);
        if(rc != SQLITE_OK){
            throw SQLError(to_string(rc) + ": Unable to bind " + value + " to the " + to_string(index) + "th argument of the statement.");
        }
    }

    //! Run one "step" (e.g. fetch one row) of a statement.
    //! If the return code is SQLITE_ROW, extra rows can be fetched by new calls to this method
    int step(){
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

    int intValue(int iCol){
        return sqlite3_column_int(preparedStatement, iCol);
    }
    const unsigned char *textValue(int iCol){
        return sqlite3_column_text(preparedStatement, iCol);
    }

private:
    sqlite3_stmt* const preparedStatement;
    sqlite3* const db;
    bool requestFinished;
};



//! A wrapper around sqlite3* objects
class SQLiteObject{
public:
    SQLiteObject(const std::string& location) :
        db(NULL)
    {
        int rc = sqlite3_open(location.c_str(), &db);
        if(rc != SQLITE_OK){
            throw SQLError("Unable to create an in-memory database");
        }
    }
    SQLiteObject(const SQLiteObject&) = delete;
    SQLiteObject& operator=(const SQLiteObject& rhs) = delete;
    ~SQLiteObject(){
        if(db){
            sqlite3_close(db);
        }
    }

    //! This is used for statements where the fetched data must be manipulated
    //! For security reasons, never concatenate user-controlled data into statement. You should rather use preparedStatements and bind arguments
    std::shared_ptr<SQLiteStatement> prepare(const std::string& statement) const{
        sqlite3_stmt* handle;
        const char* syntaxError;
        int rc = sqlite3_prepare_v2(db, statement.c_str(), statement.size(), &handle, &syntaxError);
        if(rc != SQLITE_OK){
            throw SQLError(string("Invalid statement around '") + syntaxError + "'");
        }
        return make_shared<SQLiteStatement>(db, handle);
    }

    //! This can be used for statements that do not fetch anything
    void exec(const string& statement){
        char *errmsg;
        int rc = sqlite3_exec(db, statement.c_str(), NULL, NULL, &errmsg);
        if(rc != SQLITE_OK){
            string what = string("SQL Error: ") + errmsg;
            sqlite3_free(errmsg);
            throw SQLError(what);
        }
    }

private:
    sqlite3 *db;
};

}

#endif // _SQLITE_OBJECT_HPP_