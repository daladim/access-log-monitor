#ifndef _ADDRESS_RANGE_HPP_
#define _ADDRESS_RANGE_HPP_

#include <iostream>

#include "CidrObject.hpp"
#include "Address.hpp"

namespace LogSupervisor{

class AddressRange : public CidrObject<RangeAllowed> {
public:
    // Let's inherit constructors
    using CidrObject<RangeAllowed>::CidrObject;

    typedef Address SingleParam;

};

} // namespace

#endif //_ADDRESS_RANGE_HPP_