#include "SerializerInterface.hpp"

#include <sstream>
using namespace std;


namespace LogSupervisor::Serializer{

ostream& operator<<(ostream& lhs, Interface& serializer){
    serializer.serialize(lhs);
    return lhs;
}

string Interface::to_string(){
    ostringstream ss;
    ss << *this;
    return ss.str();
}


} // namespace