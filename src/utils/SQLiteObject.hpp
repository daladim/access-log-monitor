#ifndef _SQLITE_OBJECT_HPP_
#define _SQLITE_OBJECT_HPP_

#include <sqlite3.h>
#include <memory>

namespace LogSupervisor{

class SQLError : public std::runtime_error{
public:
    SQLError(const std::string& what) : std::runtime_error(what) {}
};

//! A wrapper around sqlite3_stmt* ojects
class SQLiteStatement{
public:
    SQLiteStatement(sqlite3* db, sqlite3_stmt* const stmt);
    SQLiteStatement(const SQLiteStatement&) = delete;
    SQLiteStatement& operator=(const SQLiteStatement& rhs) = delete;
    ~SQLiteStatement();

    //! Bind a user-controlled int input to the statement
    void bindInt(int index, int value);

    //! Bind a user-controlled text input to the statement
    void bindText(int index, const std::string& value);

    //! Run one "step" (e.g. fetch one row) of a statement.
    //! If the return code is SQLITE_ROW, extra rows can be fetched by new calls to this method
    int step();

    int intValue(int iCol);
    const unsigned char *textValue(int iCol);

private:
    sqlite3_stmt* const preparedStatement;
    sqlite3* const db;
    bool requestFinished;
};



//! A wrapper around sqlite3* objects
class SQLiteObject{
public:
    SQLiteObject(const std::string& location);
    SQLiteObject(const SQLiteObject&) = delete;
    SQLiteObject& operator=(const SQLiteObject& rhs) = delete;
    ~SQLiteObject();

    //! This is used for statements where the fetched data must be manipulated
    //! For security reasons, never concatenate user-controlled data into statement. You should rather use preparedStatements and bind arguments
    std::shared_ptr<SQLiteStatement> prepare(const std::string& statement) const;

    //! This can be used for statements that do not fetch anything
    void exec(const std::string& statement);

private:
    sqlite3 *db;
};

}

#endif // _SQLITE_OBJECT_HPP_