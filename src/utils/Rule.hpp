#ifndef _RULE_HPP_
#define _RULE_HPP_

#include <iostream>
#include <optional>
#include <vector>

#include "User.hpp"
#include "AddressRange.hpp"
#include "ParamList.hpp"
#include "../database/DatabaseObjects.hpp"

namespace LogSupervisor{

//! Represents a user-defined rule
class Rule{
public:
    //! Create a Rule.<br>
    //! unset optionals mean a wildcard
    Rule(const ParamList<User, User>& users, const ParamList<AddressRange, Address> addresses, const std::string& descr, const Authentication::Validity validity) :
        users(users), addresses(addresses), descr(descr), validity(validity)
    {};

    const ParamList<User, User> users;
    const ParamList<AddressRange, Address> addresses;
    const std::string descr;
    const Authentication::Validity validity;
};

} // namespace

#endif // _RULE_HPP_