#ifndef _ADDRESS_HPP_
#define _ADDRESS_HPP_

#include "CidrObject.hpp"

namespace LogSupervisor{

//! This represents a single IPv4 or IPv6 address
class Address : public CidrObject {
public:
    //! Constructor
    Address(const std::string& str);
    //! Constructor
    Address(const char* str);
    //! Copy constructor
    Address(const Address& rhs);
    //! Assignemnt operator
    Address& operator=(const Address& rhs);

    virtual ~Address() {};

private:
    void assertValid() const; //!< Checks the sanity of the current CIDR (e.g. that it represents a single address)
};


} // namespace

#endif //_ADDRESS_HPP_