#include <algorithm>
using namespace std;

#include "DatabaseObjects.hpp"

namespace LogSupervisor{
using Validity = Authentication::Validity;

Validity::Validity(Value val) :
    val(val)
{}

Validity::Validity(const std::string& key) :
    val(Validity::Value::_Undefined)
{
    string cKey(key); // copy to non-const string
    std::transform(cKey.begin(), cKey.end(), cKey.begin(), ::tolower);
    if(cKey.compare("ok") == 0){
        val = Validity::Value::_OK;
    }
    if(cKey.compare("warning") == 0){
        val = Validity::Value::_Warning;
    }
    if(cKey.compare("critical") == 0){
        val = Validity::Value::_Critical;
    }
}

bool Validity::operator==(const Validity& rhs) const{
    return val == rhs.val;
}

Validity Validity::Undefined(){ return Validity(Validity::Value::_Undefined); }
Validity Validity::OK(){        return Validity(Validity::Value::_OK);        }
Validity Validity::Warning(){   return Validity(Validity::Value::_Warning);   }
Validity Validity::Critical(){  return Validity(Validity::Value::_Critical);  }

string Validity::to_string() const{
    switch(val){
        case Validity::Value::_Undefined:  return "?";
        case Validity::Value::_OK:         return "OK";
        case Validity::Value::_Warning:    return "warning";
        case Validity::Value::_Critical:   return "critical";
    }
    throw logic_error("Unexpected value");
}


ostream& operator<<(ostream& lhs, const Validity& rhs){
    lhs << rhs.to_string();
    return lhs;
}


} // namespace