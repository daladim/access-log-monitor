#ifndef _PARAMS_RANGE_INTERFACE_HPP_
#define _PARAMS_RANGE_INTERFACE_HPP_

namespace LogSupervisor{

template<typename SingleParam>
class ParamRangeInterface{
public:
    virtual bool contains(const SingleParam& param) const = 0;
    virtual ~ParamRangeInterface(){};
};

} // namespace

#endif //_PARAMS_RANGE_INTERFACE_HPP_