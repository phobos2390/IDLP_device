/// @file IDLP/UDP_device.h

#ifndef IDLP_UDP_DEVICE_H
#define IDLP_UDP_DEVICE_H

#include <IDLP/I_device.h>

namespace IDLP
{

class UDP_device : public IDLP::I_device
{
public:
    /// Creates class value
    UDP_device();

    /// Removes class value
    virtual ~UDP_device();

    /// @brief Writes bytes to the device
    ///
    /// @param[in] out_bytes - array of bytes to write out
    /// @param[in] bytes_size - number of bytes in the array to write to the device
    /// @param[in] timeout_ms - specified timeout, in milliseconds of the write attempt
    /// @return Two outputs:
    /// If positive or zero: returns the exact number of bytes written
    /// If negative: returns the associated IDLP error code
    virtual int64_t write(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms);

    /// @brief Reads bytes from the device
    ///
    /// @param[out] in_bytes - buffer of bytes to read
    /// @param[in] max_bytes_size - number of bytes in the array to write to the device
    /// @param[in] timeout_ms - specified timeout, in milliseconds of the read attempt
    /// @return Two outputs:
    /// If positive or zero: returns the exact number of bytes read
    /// If negative: returns the associated IDLP error code
    virtual int64_t read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms);

    IDLP::IDLP_Error set_receiving_port(uint16_t port);

    IDLP::IDLP_Error set_sending_port(uint16_t port, uint32_t ipv4_address);
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* IDLP_UDP_DEVICE_H */
