/// @file IDLP/UDP_device.cpp

#include <IDLP/UDP_device.h>

namespace IDLP
{

struct UDP_device::Impl
{
public:
    Impl(){}
    virtual ~Impl(){}

};

UDP_device::UDP_device()
    :m_p_impl(new Impl)
{
}

UDP_device::~UDP_device()
{
    delete m_p_impl;
    m_p_impl = 0;
}

}
