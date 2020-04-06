#ifndef _CIDR_OBJECT_HPP_
#define _CIDR_OBJECT_HPP_

#include <iostream>
#include <memory>
#include <cstdlib>

extern "C"{
    #include <libcidr.h>
}

namespace LogSupervisor{

//! A wrapper over a libcidr object.<br>
//! This can either allow or forbid IP ranges.
class CidrObject {
public:
    //! Constructor
    CidrObject(const char* str){
        m_cidr = cidr_from_str(str);
        if(m_cidr == NULL){
            throw std::runtime_error(std::string("Invalid address range: ") + str);
        }
    }
    //! Constructor
    CidrObject(const std::string& str) :
        CidrObject(str.c_str())
    {}

    //! Copy constructor
    CidrObject(const CidrObject& rhs){
        m_cidr = cidr_dup(rhs.m_cidr);
        if(m_cidr == NULL){
            throw std::runtime_error(std::string("Unable to copy"));
        }
    }

    //! Assignemnt operator
    CidrObject& operator=(const CidrObject& rhs){
        cidr_free(m_cidr);
        m_cidr = cidr_dup(rhs.m_cidr);
        if(m_cidr == NULL){
            throw std::runtime_error(std::string("Unable to assign"));
        }
        return *this;
    }

    virtual ~CidrObject(){
        cidr_free(m_cidr);
    }

    const CIDR* cidr() const{
        return m_cidr;
    }

    std::shared_ptr<std::string> to_string() const{
        char* str = cidr_to_str(m_cidr, CIDR_NOFLAGS);
        std::shared_ptr<std::string> output = std::make_shared<std::string>(str);
        free(str);
        return output;
    }

protected:
    CIDR* m_cidr;
};


} // namespace

#endif //_CIDR_OBJECT_HPP_