#ifndef _DATABASE_OBJECTS_HPP_
#define _DATABASE_OBJECTS_HPP_

#include "../utils/User.hpp"
#include "../utils/Address.hpp"
#include "../utils/Timestamp.hpp"


namespace LogSupervisor{

//! This class defines a "row" in the database of stored authentications
class Authentication{
public:
    enum Validity{
        Undefined,
        OK,
        Warning,
        Error
    };

    Authentication(const User& user, const Address& origin, const Timestamp& ts) :
        user(user), origin(origin), timestamp(ts),
        validity(Undefined)
    {}

    const User user;
    const Address origin;
    const Timestamp timestamp;
    Validity validity;
};


} // namespace

#endif //_DATABASE_OBJECTS_HPP_