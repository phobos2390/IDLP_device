/// @file IDLP/UDP_device.h

#ifndef IDLP_UDP_DEVICE_H
#define IDLP_UDP_DEVICE_H

namespace IDLP
{

class UDP_device
{
public:
    /// Creates class value
    UDP_device();

    /// Removes class value
    virtual ~UDP_device();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* IDLP_UDP_DEVICE_H */
