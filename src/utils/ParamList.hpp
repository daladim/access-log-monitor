#ifndef _PARAMS_LIST_HPP_
#define _PARAMS_LIST_HPP_

#include <iostream>
#include <vector>

#include "AddressRange.hpp"
#include "Address.hpp"

namespace LogSupervisor{

//! This stores a list of params, and provides a few utilities methods<br>
//! Each list can contain:<br>
//!  * no value (in this case, this acts like a wildcard)
//!  * a single value
//!  * multiple values
template<typename Param, typename ComparedParam>
class ParamList{
public:
    //! Construct a "wildcard"
    ParamList() :
        isWildcard(true)
    {}

    //! Constructs a list with one or multiple params
    ParamList(const std::vector<Param>& params) :
        m_params(params), isWildcard(false)
    {}

    void insert(const std::vector<Param>& params){
        isWildcard = (params.size() == 0);
        m_params = params;
    }

    bool contains(const ComparedParam& param) const{
        if(isWildcard){ return true; }
        for(const Param& contained : m_params){
            if(containCheck(contained, param)){
                return true;
            }
        }
        return false;
    }

private:
    bool isWildcard;
    std::vector<Param> m_params;

    //! Default comparator, that can be specialized if needed
    bool containCheck(const Param& lhs, const ComparedParam& rhs) const{
        return lhs == rhs;
    }

};


//! Specialization for AddressRanges
template<>
bool ParamList<AddressRange, Address>::containCheck(const AddressRange& lhs, const Address& rhs) const;

} // namespace

#endif //_PARAMS_LIST_HPP_