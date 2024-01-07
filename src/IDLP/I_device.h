/// @file IDLP/I_device.h
/// @brief Interface for any byte oriented device to work with

#include <cstdint>
#include <cstdlib>
#include <IDLP/IDLP_Errors.h>

#ifndef IDLP_I_DEVICE_H
#define IDLP_I_DEVICE_H

namespace IDLP
{

class I_device
{
public:
    /// Creates class value
    I_device(){}

    /// Removes class value
    virtual ~I_device(){}

    /// @brief Writes bytes to the device
    ///
    /// @param[in] out_bytes - array of bytes to write out
    /// @param[in] bytes_size - number of bytes in the array to write to the device
    /// @param[in] timeout_ms - specified timeout, in milliseconds of the write attempt
    /// @return Two outputs:
    /// If positive or zero: returns the exact number of bytes written
    /// If negative: returns the associated IDLP error code
    virtual int64_t write(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms)=0;

    /// @brief Writes bytes to the device
    ///
    /// @param[in] out_bytes - array of bytes to write out
    /// @param[in] expected_bytes_size - number of bytes in the array to write to the device
    /// @param[in] timeout_ms - specified timeout, in milliseconds of the write attempt
    /// @return The associated IDLP Error code
    virtual IDLP::IDLP_Error write_exact(uint8_t* out_bytes, size_t expected_bytes_size, size_t timeout_ms);

    /// @brief Reads bytes from the device
    ///
    /// @param[out] in_bytes - buffer of bytes to read
    /// @param[in] max_bytes_size - number of bytes in the array to write to the device
    /// @param[in] timeout_ms - specified timeout, in milliseconds of the read attempt
    /// @return Two outputs:
    /// If positive or zero: returns the exact number of bytes read
    /// If negative: returns the associated IDLP error code
    virtual int64_t read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)=0;

    /// @brief Reads bytes from the device
    ///
    /// @param[out] in_bytes - buffer of bytes to read
    /// @param[in] expected_bytes_size - number of bytes in the array to write to the device
    /// @param[in] timeout_ms - specified timeout, in milliseconds of the read attempt
    /// @return The IDLP Error code
    virtual IDLP::IDLP_Error read_exact(uint8_t* in_bytes, size_t expected_bytes_size, size_t timeout_ms);
};

}

#endif /* IDLP_I_DEVICE_H */
