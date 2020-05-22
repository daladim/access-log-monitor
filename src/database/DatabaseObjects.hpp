#ifndef _DATABASE_OBJECTS_HPP_
#define _DATABASE_OBJECTS_HPP_

#include <optional>
#include <string>
#include <ostream>

#include "../utils/User.hpp"
#include "../utils/Address.hpp"
#include "../utils/Timestamp.hpp"


namespace LogSupervisor{

//! This class defines a "row" in the database of stored authentications
class Authentication{
public:
    //! An enum with associated string values
    class Validity{
    public:
        Validity(const std::string& key = "");
        static Validity Undefined();
        static Validity OK();
        static Validity Warning();
        static Validity Critical();
        std::string to_string() const;
        bool operator==(const Validity& rhs) const;
        friend std::ostream& operator<<(std::ostream& lhs, const Validity& rhs);

    private:
        enum Value{
            _Undefined=0,
            _OK=1,
            _Warning=2,
            _Critical=3,
        };

        Validity(Value val);
        Value val;
    };

    // I am too lazy to create a .cpp file just to define a static const string, so preprocessor macros will do
    #define DEFAULT_DESCR "<no description available>"

    Authentication(const User& user, const Address& origin, const Timestamp& ts, bool success,
                   unsigned int count = 0,
                   Validity val = Validity::Undefined(), const std::string& descr = DEFAULT_DESCR, std::optional<int> id = {}) :
        id(id),
        user(user), origin(origin), timestamp(ts), success(success),
        count(count),
        description(descr),
        validity(val)
    {}

    const std::optional<int> id;
    const User user;            //!< The user this authentication concerns
    const Address origin;       //!< The remote IP this connection comes from
    const Timestamp timestamp;  //!< When this authentication happened
    const bool success;         //!< whether the authentication attempt succeeded (it may have failed, e.g. if a wrong password was presented)
    unsigned int count;         //!< How many times this couple (user,address,validity...) has been detected
    std::string description;    //!< A place to store a user-defined description
    Validity validity;          //!< A place to store what we think about this authentication
};


} // namespace

#endif //_DATABASE_OBJECTS_HPP_