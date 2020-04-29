#include "ParamList.hpp"

namespace LogSupervisor{

template<>
bool ParamList<AddressRange, Address>::containCheck(const AddressRange& lhs, const Address& rhs) const{
    return lhs.contains(rhs);
}

} // namespace
