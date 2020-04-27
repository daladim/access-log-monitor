#ifndef _SQLITE_STATEMENT_ITERATOR_HPP_
#define _SQLITE_STATEMENT_ITERATOR_HPP_

#include <memory>

#include "SQLiteDB.hpp"

namespace SQLite{

//! This C++ kind-of-iterator runs step() on an underlying SQLiteStatement at every incrementation.<br>
//! Note that every copy of an iterator acts (and modifies) the same underlying statement.
class StatementIterator{
public:
    // iterator traits
    using iterator_category = std::forward_iterator_tag;
    using value_type = SQLite::Statement;
    using pointer = SQLite::Statement*;
    using reference = SQLite::Statement&;
    //using difference_type = long;

    StatementIterator& operator=(const StatementIterator&) = delete;           // Not sure modifying an iterator makes sense

    //! Increment the iterator, calling step() on the underlying Statement
    SQLite::Statement& operator++();
    SQLite::Statement operator++(int) = delete; // I'm too lazy to implement the postfix incrementation operator (prefix incrementation should be enough), but it *might* be doable
    const SQLite::Statement& operator*();
    const SQLite::Statement* operator->();
    bool operator==(const StatementIterator& other) const;
    bool operator!=(const StatementIterator& other) const;

    friend class Statement;

private:
    SQLite::Statement& statement;

    //! Create an iterator.<br>
    //! It will act on (and modify) the underlying SQLite::Statement. Thus, it is not advised to have multiple StatementIterator for the same underlying Statement.<br>
    //! This is why this class hides its constructor. Usually, only SQLite::Statement::begin() is supposed to create StatementIterator.
    StatementIterator(SQLite::Statement& statement, bool thisIsAnEndIteratorOnly=false);

    bool thisIsAnEndIteratorOnly;
    bool isAtEnd() const;
};

} // namespace

#endif // _SQLITE_STATEMENT_ITERATOR_HPP_