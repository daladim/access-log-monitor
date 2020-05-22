#ifndef _ALIAS_STORE_HPP_
#define _ALIAS_STORE_HPP_

#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <initializer_list>

namespace LogSupervisor{
//! This class helps store lists of values and resolving aliases.<br>
//! In can be fed with aliases to values or to other aliases.<br>
//! In can later be used to retrieve the list of values associated to a given alias.<br>
//! Example:
//! ~~~~~~~~~~~~~~~{.cpp}
//! AliasStore<Sprocket> s;
//! s.insert("positive integers", {" 1"," 2"," 3"," 4"});
//! s.insert("negative integers", {"-1","-2","-3","-4"});
//! s.insert("integers", {"positive integers", "negative integers"});
//!
//! s.at("positive integers");  // returns { 1, 2, 3, 4}
//! s.at("integers");           // returns { 1, 2, 3, 4,-1,-2,-3,-4}
//! ~~~~~~~~~~~~~~~
//!
//! The Content template can be anything constructible from a std::string
//! In case Content is std::string, then it is not possible for a final value to have the same name as an alias.
template<typename Content>
class AliasStore{
public:
    //! Define a new alias
    void insert(const std::string& alias, const std::vector<std::string>& values){
        if(aliases.find(alias) != aliases.end()){
            throw std::range_error("This alias is already defined");
        }

        std::vector<Content> list;
        for(const std::string& value : values){
            try{
                // Add either an existing alias...
                std::vector<Content>& aliased = aliases.at(value);
                list.insert(list.end(), aliased.begin(), aliased.end() );
            }catch(const std::out_of_range&){
                // ...or a new Content object
                try{
                    list.push_back( Content(value) );
                }catch(const std::exception& exc){
                    throw std::range_error(std::string("Unable to create an item from ") + value + ":" + exc.what());
                }
            }
        }
        aliases[alias] = list;
    }

    //!< Returns the value at a given key, or construct from the given argument in case none exists.
    const std::vector<Content> parse(const std::string& key) const{
        try{
            // Return either an existing alias...
            return aliases.at(key);
        }catch(const std::out_of_range&){
            // ...or a new Content object
            try{
                return{ Content(key) };
            }catch(const std::exception& exc){
                throw std::range_error(std::string("Unable to create an item from ") + key + ":" + exc.what());
            }
        }
    }

#ifdef DEBUG
    void print() const{
        std::cout << std::left << std::setw(10) << "alias" << "value" << std::endl;
        for(const auto& pair : aliases){
            std::cout << std::left << std::setw(10) << pair.first;
            for(const auto& item : pair.second){
                std::cout << item << ",";
            }
            std::cout << std::endl;
        }
    }
#endif

private:
    std::unordered_map<std::string, std::vector<Content>> aliases;
};

} // namespace

#endif //_ALIAS_STORE_HPP_