#ifndef _DATABASE_REQUEST_HPP_
#define _DATABASE_REQUEST_HPP_

#include <memory>

#include "DatabaseObjects.hpp"
#include "../utils/sqlite/SQLiteStatement.hpp"

namespace LogSupervisor::Database{

class Request{
public:
    class Iterator; // defined below

    //! Construct a request, and retain the underlying statement
    Request(std::shared_ptr<SQLite::Statement> statement);

    //! Create an iterator. It is not valid if the parent Request is destroyed.
    Iterator begin();
    //! End iterator. It is not valid if the parent Request is destroyed.
    Iterator end();

    //! Get the first row. This is useful in case you are sure the request has a single result
    std::shared_ptr<Authentication> firstRow();

private:
    std::shared_ptr<SQLite::Statement> statement;







public:
    //! Iterator to retrieve records from the Database
    class Iterator{
    public:
        // iterator traits
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::shared_ptr<LogSupervisor::Authentication>;
        using pointer = std::shared_ptr<LogSupervisor::Authentication>;
        using reference = std::shared_ptr<LogSupervisor::Authentication>;
        //using difference_type = long;

        //! Create an iterator<br>
        //! This object is not valid if the underlying SQLite::Statement::Iterator is destroyed.
        Iterator(SQLite::Statement::Iterator sqlsi);

        std::shared_ptr<LogSupervisor::Authentication> operator++();
        std::shared_ptr<LogSupervisor::Authentication> operator++(int) = delete; // I'm too lazy to implement the postfix incrementation operator (prefix incrementation should be enough), but it *might* be doable
        const std::shared_ptr<LogSupervisor::Authentication> operator*();
        const std::shared_ptr<LogSupervisor::Authentication> operator->();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        SQLite::Statement::Iterator sqlsi;
        std::shared_ptr<LogSupervisor::Authentication> currentAuth();
    };
};

} // namespace

#endif //_DATABASE_REQUEST_HPP_