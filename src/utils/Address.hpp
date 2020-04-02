#ifndef _ADDRESS_HPP_
#define _ADDRESS_HPP_

#include "CidrObject.hpp"

namespace LogSupervisor{

//! This represents a single IPv4 or IPv6 address
typedef CidrObject<RangeForbidden> Address;

} // namespace

#endif //_ADDRESS_HPP_