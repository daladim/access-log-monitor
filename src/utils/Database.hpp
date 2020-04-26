#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <vector>

#include "sqlite/SQLiteDB.hpp"
#include "User.hpp"
#include "Address.hpp"
#include "Timestamp.hpp"


namespace LogSupervisor{

enum Validity{
    Undefined,
    OK,
    Warning,
    Error
};

//! This class defines a "row" in the database of stored authentications
class Authentication{
public:
    Authentication(const User& user, const Address& origin, const Timestamp& ts) :
        user(user), origin(origin), timestamp(ts),
        validity(Undefined)
    {}

    const User user;
    const Address origin;
    const Timestamp timestamp;
    Validity validity;
};


//! This class stores detected logins
//!
//! They can be sorted by different keys before they can be retrieved
class Database{
public:
    Database();

    void insert(const Authentication& auth);
    std::vector<std::shared_ptr<const Authentication>> all() const;

private:
    SQLite::SQLiteDB db;
};

} // namespace

#endif //_DATABASE_HPP_