#ifndef _SQLITE_STATEMENT_HPP_
#define _SQLITE_STATEMENT_HPP_

#include <sqlite3.h>
#include <memory>

#include "SQLite.hpp"

namespace SQLite{

//! A wrapper around sqlite3_stmt* ojects
class Statement{
public:
    class Iterator; // defined later in this file

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
    //! Note: for better convenience, you may want to use a Statement::Iterator instead
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
    Statement::Iterator begin();
    //! Returns a way to check for the last step of an iterator
    const Statement::Iterator end();

private:
    sqlite3_stmt* const preparedStatement;
    sqlite3* const db;
    bool hasStepped;
    bool requestFinished;

    Statement::Iterator* currentIterator;







public:
    //! This C++ kind-of-iterator runs step() on an underlying SQLiteStatement at every incrementation.<br>
    //! Note that every copy of an iterator acts (and modifies) the same underlying statement.
    class Iterator{
    public:
        // iterator traits
        using iterator_category = std::forward_iterator_tag;
        using value_type = SQLite::Statement;
        using pointer = SQLite::Statement*;
        using reference = SQLite::Statement&;
        //using difference_type = long;

        Iterator& operator=(const Iterator&) = delete;           // Not sure modifying an iterator makes sense

        //! Increment the iterator, calling step() on the underlying Statement
        SQLite::Statement& operator++();
        SQLite::Statement operator++(int) = delete; // I'm too lazy to implement the postfix incrementation operator (prefix incrementation should be enough), but it *might* be doable
        const SQLite::Statement& operator*();
        const SQLite::Statement* operator->();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

        friend class Statement;

    private:
        SQLite::Statement& statement;

        //! Create an iterator.<br>
        //! It will act on (and modify) the underlying SQLite::Statement. Thus, it is not advised to have multiple Iterator for the same underlying Statement.<br>
        //! This is why this class hides its constructor. Usually, only SQLite::Statement::begin() is supposed to create Iterator.
        Iterator(SQLite::Statement& statement, bool thisIsAnEndIteratorOnly=false);

        bool thisIsAnEndIteratorOnly;
        bool isAtEnd() const;
    };
};

} // namespace

#endif // _SQLITE_STATEMENT_HPP_