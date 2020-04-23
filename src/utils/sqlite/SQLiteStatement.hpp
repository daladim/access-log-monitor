#ifndef _SQLITE_STATEMENT_HPP_
#define _SQLITE_STATEMENT_HPP_

#include <sqlite3.h>
#include <memory>

#include "SQLite.hpp"

namespace SQLite{

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
    //! Note: for better convenience, you may want to use a SQLiteStatementIterator instead
    int step();

    //! Return an int value from the current row
    int intValue(int iCol);
    //! Return an string value from the current row
    const unsigned char *textValue(int iCol);

    //! A getter for the internal state of the statement
    bool isExhausted() const { return requestFinished; }

private:
    sqlite3_stmt* const preparedStatement;
    sqlite3* const db;
    bool requestFinished;
};

} // namespace

#endif // _SQLITE_STATEMENT_HPP_