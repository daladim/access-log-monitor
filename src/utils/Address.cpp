#include <cstring>

#include "Address.hpp"
using namespace std;

namespace LogSupervisor{

Address::Address(const std::string& str) :
    CidrObject(str)
{
    assertValid();
}

Address::Address(const char* str) :
    CidrObject(str)
{
    assertValid();
}

Address::Address(const Address& rhs) :
    CidrObject(rhs)
{
    assertValid();
}

Address& Address::operator=(const Address& rhs){
    CidrObject::operator=(rhs);
    assertValid();
    return *this;
}

void Address::assertValid() const{
    if(strncmp( "1", cidr_numaddr(m_cidr), 2)){
        throw range_error("Invalid IP (it should be a valid, unique IP address).");
    }
}

} // namespace