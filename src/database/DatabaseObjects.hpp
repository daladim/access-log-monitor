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
        Critical
    };

    Authentication(const User& user, const Address& origin, const Timestamp& ts, bool success,
                   Validity val = Validity::Undefined, std::optional<int> id = {}) :
        id(id),
        user(user), origin(origin), timestamp(ts), success(success),
        validity(val)
    {}

    const std::optional<int> id;
    const User user;            //!< The user this authentication concerns
    const Address origin;       //!< The remote IP this connection comes from
    const Timestamp timestamp;  //!< When this authentication happened
    const bool success;         //!< whether the authentication attempt succeeded (it may have failed, e.g. if a wrong password was presented)
    Validity validity;          //!< A place to store what we think about this authentication
};


} // namespace

#endif //_DATABASE_OBJECTS_HPP_