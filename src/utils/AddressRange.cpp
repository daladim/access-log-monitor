#include "AddressRange.hpp"
using namespace std;

namespace LogSupervisor{


AddressRange::AddressRange(const std::string& str) :
    CidrObject(str)
{}

AddressRange::AddressRange(const char* str) :
    CidrObject(str)
{}

AddressRange::AddressRange(const AddressRange& rhs) :
    CidrObject(rhs)
{}

AddressRange& AddressRange::operator=(const AddressRange& rhs)
{
    CidrObject::operator=(rhs);
    return *this;
}


bool AddressRange::contains(const Address& param) const{
    return cidr_contains(m_cidr, param.cidr()) == 0;
}

} // namespace