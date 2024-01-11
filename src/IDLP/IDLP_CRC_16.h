/// @file IDLP/IDLP_CRC_16.h

#ifndef IDLP_IDLP_CRC_16_H
#define IDLP_IDLP_CRC_16_H

namespace IDLP
{

class IDLP_CRC_16
{
public:
    /// Creates class value
    IDLP_CRC_16();

    /// Removes class value
    virtual ~IDLP_CRC_16();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* IDLP_IDLP_CRC_16_H */
