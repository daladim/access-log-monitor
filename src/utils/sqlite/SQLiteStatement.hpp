#ifndef _SQLITE_STATEMENT_HPP_
#define _SQLITE_STATEMENT_HPP_

#include <sqlite3.h>
#include <memory>

#include "SQLite.hpp"

namespace SQLite{

class StatementIterator; // defined elsewhere

//! A wrapper around sqlite3_stmt* ojects
class Statement{
public:
    Statement(sqlite3* db, sqlite3_stmt* const stmt);
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement& rhs) = delete;
    ~Statement();

    //! Bind a user-controlled int input to the statement. index starts at 1, not 0
    void bindInt(int index, int value);

    //! Bind a user-controlled text input to the statement. index starts at 1, not 0
    void bindText(int index, const std::string& value);

    //! Run one "step" (e.g. fetch one row) of a statement.
    //! If the return code is SQLITE_ROW, extra rows can be fetched by new calls to this method
    //! Note: for better convenience, you may want to use a StatementIterator instead
    int step();

    //! Return an int value from the current row, or throw if no row is available
    int intValue(int iCol) const;
    //! Return an string value from the current row, or throw if no row is available
    const unsigned char *textValue(int iCol) const;

    //! A getter for the internal state of the statement
    bool hasStarted() const { return hasStepped; }
    //! A getter for the internal state of the statement
    bool isExhausted() const { return requestFinished; }

    //! Returns a StatementInterator for this statement.
    //! Since it is not possible to "rewind" a statement, it is only possible to call this function once for every Statement (this will throw otherwise)
    StatementIterator begin();
    //! Returns a way to check for the last step of an iterator
    const StatementIterator end();

private:
    sqlite3_stmt* const preparedStatement;
    sqlite3* const db;
    bool hasStepped;
    bool requestFinished;

    StatementIterator* currentIterator;
};

} // namespace

#endif // _SQLITE_STATEMENT_HPP_