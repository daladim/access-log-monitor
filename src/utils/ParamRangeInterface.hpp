#ifndef _PARAMS_RANGE_INTERFACE_HPP_
#define _PARAMS_RANGE_INTERFACE_HPP_

namespace LogSupervisor{

//! This is the interface for parameters that are ranges (e.g. range of IP addresses, time intervals...)
template<typename SingleParam>
class ParamRangeInterface{
public:
    virtual bool contains(const SingleParam& param) const = 0;
    virtual ~ParamRangeInterface(){};
};

} // namespace

#endif //_PARAMS_RANGE_INTERFACE_HPP_