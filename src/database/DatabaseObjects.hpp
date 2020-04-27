#ifndef _DATABASE_OBJECTS_HPP_
#define _DATABASE_OBJECTS_HPP_

#include <optional>

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

    Authentication(const User& user, const Address& origin, const Timestamp& ts, Validity val = Validity::Undefined, std::optional<int> id = {}) :
        id(id),
        user(user), origin(origin), timestamp(ts),
        validity(val)
    {}

    const std::optional<int> id;
    const User user;
    const Address origin;
    const Timestamp timestamp;
    Validity validity;
};


} // namespace

#endif //_DATABASE_OBJECTS_HPP_