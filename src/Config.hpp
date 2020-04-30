#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <iostream>
#include <vector>

#include <yaml-cpp/yaml.h> // in the libs/ folder

#include "utils/Rule.hpp"
#include "utils/AliasStore.hpp"
#include "utils/AddressRange.hpp"
#include "utils/User.hpp"
#include "utils/Timestamp.hpp"
#include "database/DatabaseObjects.hpp"

namespace LogSupervisor{

//! Parses and stores a config
class Config{
public:
    //! Parse a config file
    Config(const std::string& configPath);

    //! Retrieve the current rules
    const std::vector<Rule>& rules() const;

    //! Retrieve the default status as defined in the config file
    const Authentication::Validity default_status() const{ return default_validity; };


private:
    AliasStore<AddressRange> addrAliases;
    AliasStore<User> userAliases;
    std::vector<Rule> m_rules;
    Authentication::Validity default_validity;

    void populateRules(const YAML::Node& yamlRules);
};

} // namespace

#endif //_CONFIG_HPP_