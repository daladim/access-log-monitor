#include <iostream>
#include <fstream>
using namespace std;

#include "Config.hpp"
#include <yaml-cpp/yaml.h> // in the libs/ folder

namespace LogSupervisor{

Config::Config(const string& configFile) :
    default_validity(Authentication::Validity::Undefined())
{
    try{
        parseConfig(configFile);
    }catch(const YAML::Exception& e){
        cerr << "ERROR: unable to parse the file at " << configFile << endl;
        cerr << e.what() << endl;
        throw e;
    }
}

void Config::parseConfig(const string& configFile){
    ifstream file(configFile);
    YAML::Node config = YAML::LoadFile(configFile);

    default_validity = config["default_status"].as<Authentication::Validity>();
    if(config["mail"]){
        m_mailAddress.emplace(config["mail"].as<string>());
    }

    addrAliases = config["addresses"].as<AliasStore<AddressRange>>();
    userAliases = config["users"].as<AliasStore<User>>();

    populateRules(config["rules"]);
}

const std::vector<Rule>& Config::rules() const{
    return m_rules;
}

// It is not handy to implement convert<Rule>::decode() (it would need a reference to our AliasStore objects for instance),
// so let's do this in a specific function
void Config::populateRules(const YAML::Node& yamlRules){
    for(const YAML::Node rule : yamlRules){
        ParamList<User, User> users;
        ParamList<AddressRange, Address> addrs;

        unsigned int nValidCriteria = 0;
        if(rule["users"]){
            users.insert(userAliases.parse(rule["users"].as<string>()));
            ++nValidCriteria;
        }
        if(rule["addresses"]){
            addrs.insert(addrAliases.parse(rule["addresses"].as<string>()));
            ++nValidCriteria;
        }
        string descr = rule["description"].as<string>();
        Authentication::Validity val = rule["status"].as<Authentication::Validity>();

        if(nValidCriteria == 0){
            cerr << "Warning: the following rule does not have any valid criteria. Are you sure about it? " << rule << endl;
        }

        m_rules.push_back(Rule(users, addrs, descr, val));
    }
}





} // namespace


using namespace LogSupervisor;
namespace YAML{

    //! Template specializations so that YAML can "natively" use AliasStore objects
    template<typename T>
    struct convert<AliasStore<T>>{
        static Node encode(const AliasStore<T>& store){
            throw logic_error("Serializing this object to yaml is not implemented");
            Node node;
            return node;
        }

        static bool decode(const Node& node, AliasStore<T>& rhs){
            for(YAML::const_iterator it=node.begin(); it!=node.end(); ++it){
                rhs.insert(it->first.as<string>(), it->second.as<vector<string>>());
            }
            return true;
        }
    };

    //! Template specializations so that YAML can "natively" use Validity objects
    template<>
    struct convert<Authentication::Validity>{
        static Node encode(const Authentication::Validity& va){
            throw logic_error("Serializing this object to yaml is not implemented");
            Node node;
            return node;
        }

        static bool decode(const Node& node, Authentication::Validity& rhs){
            string sVal = node.as<string>();
            try{
                rhs = Authentication::Validity(sVal);
                return true;
            }catch(...){
                return false;
            }
            return false;
        }
    };

}
