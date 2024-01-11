/// @file IDLP/IDLP_default_headers.h

#ifndef IDLP_IDLP_DEFAULT_HEADERS_H
#define IDLP_IDLP_DEFAULT_HEADERS_H

namespace IDLP
{

class IDLP_default_headers
{
public:
    /// Creates class value
    IDLP_default_headers();

    /// Removes class value
    virtual ~IDLP_default_headers();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* IDLP_IDLP_DEFAULT_HEADERS_H */
