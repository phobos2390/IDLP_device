/// @file IDLP/IDLP_default_headers.cpp

#include <IDLP/IDLP_default_headers.h>

namespace IDLP
{

struct IDLP_default_headers::Impl
{
public:
    Impl(){}
    virtual ~Impl(){}
};

IDLP_default_headers::IDLP_default_headers()
    :m_p_impl(new Impl)
{
}

IDLP_default_headers::~IDLP_default_headers()
{
    delete m_p_impl;
    m_p_impl = 0;
}

}
