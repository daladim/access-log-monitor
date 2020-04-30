#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "../utils/sqlite/SQLiteDB.hpp"
#include "DatabaseObjects.hpp"
#include "DatabaseRequest.hpp"

namespace LogSupervisor::Database{

//! This class stores detected logins
//!
//! They can be sorted by different keys before they can be retrieved
class Database{
public:
    Database();

    //! Insert a record into the DB
    void insert(const Authentication& auth);

    //! Update the validity field of a record
    void updateValidity(int id, Authentication::Validity v);

    //! Retrieve all the records: begin iterator
    Request all() const;

    //! Retrieve a single record
    std::shared_ptr<Authentication> fetch(int id);

private:
    SQLite::DB db;
};

} // namespace

#endif //_DATABASE_HPP_