#ifndef _SERIALIZER_INTERFACE_HPP_
#define _SERIALIZER_INTERFACE_HPP_

#include <iostream>
#include <ostream>

#include "../database/Database.hpp"

namespace LogSupervisor::Serializer{

//! This describes what the serializers are up to<br>
//! Serializer objects are usually constrcted with a const reference to a Database
class Interface{
public:
    //! Serialize the object.<br>
    //! This will be called when calling ostream::operator<<(Interface&)
    virtual std::ostream& serialize(std::ostream& lhs) = 0;



private:
    friend std::ostream& operator<<(std::ostream& lhs, Interface& serializer);
};

} // namespace

#endif //_SERIALIZER_INTERFACE_HPP_