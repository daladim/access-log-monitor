#ifndef _ADDRESS_RANGE_HPP_
#define _ADDRESS_RANGE_HPP_

#include <iostream>

#include "CidrObject.hpp"
#include "ParamRangeInterface.hpp"
#include "Address.hpp"

namespace LogSupervisor{

class AddressRange : public CidrObject, public ParamRangeInterface<Address> {
public:
    //! Constructor
    AddressRange(const std::string& str);
    //! Constructor
    AddressRange(const char* str);
    //! Copy constructor
    AddressRange(const AddressRange& rhs);
    //! Assignemnt operator
    AddressRange& operator=(const AddressRange& rhs);

    virtual ~AddressRange() {};

    bool contains(const Address& param) const override;
};

} // namespace

#endif //_ADDRESS_RANGE_HPP_