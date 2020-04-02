#ifndef _CIDR_OBJECT_HPP_
#define _CIDR_OBJECT_HPP_

#include <iostream>
#include <memory>
#include <cstdlib>

extern "C"{
    #include <libcidr.h>
}

namespace LogSupervisor{

//! Specifies whether a CidrObject class can contain a range, or must be a unique addresse
enum CidrObjectType {
    RangeAllowed = 0,
    RangeForbidden = 1
};


//! A wrapper over a libcidr object.<br>
//! This can either allow or forbid IP ranges.
template<CidrObjectType Type>
class CidrObject {
public:
    //! Constructor
    CidrObject(const std::string& str){ CidrObject(str.c_str()); }
    //! Constructor
    CidrObject(const char* str){
        m_cidr = cidr_from_str(str);
        if(m_cidr == NULL){
            throw std::runtime_error(std::string("Invalid address range: ") + str);
        }
        assertRightType();
    }

    //! Copy constructor
    CidrObject(const CidrObject& rhs){
        m_cidr = cidr_dup(rhs.m_cidr);
        if(m_cidr == NULL){
            throw std::runtime_error(std::string("Unable to copy"));
        }
        assertRightType();
    }

    //! Assignemnt operator
    CidrObject& operator=(const CidrObject& rhs){
        cidr_free(m_cidr);
        m_cidr = cidr_dup(rhs.m_cidr);
        if(m_cidr == NULL){
            throw std::runtime_error(std::string("Unable to assign"));
        }
        assertRightType();
    }

    virtual ~CidrObject(){
        cidr_free(m_cidr);
    }

    std::shared_ptr<std::string> to_string() const{
        char* str = cidr_to_str(m_cidr, CIDR_NOFLAGS);
        std::shared_ptr<std::string> output = std::make_shared<std::string>(str);
        free(str);
        return output;
    }

    template<CidrObjectType OtherType>
    bool contains(const CidrObject<OtherType>& rhs) const{
        if(Type == RangeForbidden){
            throw std::logic_error("Single address cannot contain anything");
        }
        return(cidr_contains(m_cidr, rhs.m_cidr) == 0);
    }

    // All CidrObject classes are friends with one another
    template<CidrObjectType OtherType> friend class CidrObject;

protected:
    CIDR* m_cidr;

private:
    //!< Checks that the current cidr actually is the right CidrObjectType, or throw
    void assertRightType() const{
        if(Type == RangeForbidden && strncmp(cidr_numaddr(m_cidr), "1", 2) != 0){
            throw std::runtime_error("Invalid CIDR input");
        }
    }
};


} // namespace

#endif //_CIDR_OBJECT_HPP_