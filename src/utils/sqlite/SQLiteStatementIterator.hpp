#ifndef _SQLITE_STATEMENT_ITERATOR_HPP_
#define _SQLITE_STATEMENT_ITERATOR_HPP_

#include "SQLiteDB.hpp"

namespace SQLite{

//! This C++ iterator runs step() on an underlying SQLiteStatement at every incrementation.
//!
//! StatementIterator act on (and modify) their underlying SQLite::Statement
class StatementIterator{
public:
    // iterator traits
    using iterator_category = std::forward_iterator_tag;
    using value_type = SQLite::Statement;
    using pointer = SQLite::Statement*;
    using reference = SQLite::Statement&;
    //using difference_type = long;

    //! Create an iterator. This will act on (and modify) the underlying SQLiteStatement
    StatementIterator(std::shared_ptr<SQLite::Statement> statement);

    StatementIterator(const StatementIterator&) = delete;   // Not sure copying an iterator makes sense since every copy would modifiy the same underlying statement
    StatementIterator& operator=(const StatementIterator&) = delete;           // Not sure modifying an iterator makes sense

    //! Increment the iterator, calling step() on the underlying Statement
    SQLite::Statement& operator++();
    SQLite::Statement operator++(int) = delete; // I'm too lazy to implement the postfix incrementation operator (prefix incrementation should be enough), but it *might* be doable
    bool operator==(StatementIterator other) const { return statement == other.statement; };
    bool operator!=(StatementIterator other) const { return statement != other.statement; };
    const SQLite::Statement& operator*();
    const SQLite::Statement* operator->();

private:
    std::shared_ptr<SQLite::Statement> statement;
};

} // namespace

#endif // _SQLITE_STATEMENT_ITERATOR_HPP_