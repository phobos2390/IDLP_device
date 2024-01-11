/// @file IDLP/IDLP_CRC_16.cpp

#include <IDLP/IDLP_CRC_16.h>

namespace IDLP
{

struct IDLP_CRC_16::Impl
{
public:
    Impl(){}
    virtual ~Impl(){}
};

IDLP_CRC_16::IDLP_CRC_16()
    :m_p_impl(new Impl)
{
}

IDLP_CRC_16::~IDLP_CRC_16()
{
    delete m_p_impl;
    m_p_impl = 0;
}

}
