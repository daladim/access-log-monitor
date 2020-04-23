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

    //! Create an iterator.
    //! It will act on (and modify) the underlying SQLite::Statement
    //! It is no longer valid if the parent SQLite::Statement is destroyed.
    StatementIterator(SQLite::Statement& statement, bool thisIsAnEndIteratorOnly=false);

    StatementIterator(const StatementIterator&) = delete;   // Not sure copying an iterator makes sense since every copy would modifiy the same underlying statement
    StatementIterator& operator=(const StatementIterator&) = delete;           // Not sure modifying an iterator makes sense

    //! Increment the iterator, calling step() on the underlying Statement
    SQLite::Statement& operator++();
    SQLite::Statement operator++(int) = delete; // I'm too lazy to implement the postfix incrementation operator (prefix incrementation should be enough), but it *might* be doable
    const SQLite::Statement& operator*();
    const SQLite::Statement* operator->();
    bool operator==(const StatementIterator& other) const;
    bool operator!=(const StatementIterator& other) const;


private:
    SQLite::Statement& statement;

    bool thisIsAnEndIteratorOnly;
    bool isAtEnd() const;
};

} // namespace

#endif // _SQLITE_STATEMENT_ITERATOR_HPP_