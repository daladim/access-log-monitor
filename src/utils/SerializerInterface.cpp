#include "SerializerInterface.hpp"

namespace LogSupervisor::Serializer{

std::ostream& operator<<(std::ostream& lhs, Interface& serializer){
    serializer.serialize(lhs);
    return lhs;
}

} // namespace