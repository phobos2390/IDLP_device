/// @file IDLP/File_device.h

#ifndef IDLP_FILE_DEVICE_H
#define IDLP_FILE_DEVICE_H

#include <memory>
#include <IDLP/I_device.h>
#include <IDLP/IDLP_Errors.h>

namespace IDLP
{

class File_device : public IDLP::I_device
{
public:
    /// Creates class value
    File_device();

    /// Removes class value
    virtual ~File_device();

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

    /// @brief Opens file
    ///
    /// @param[in] filename - path to the file to open
    /// @param[in] flags - mechanism by which to open the file
    /// @return errors
    IDLP::IDLP_Error open_file(std::string filename, int32_t flags, int32_t mode);
private:
    struct Impl;
    std::unique_ptr<Impl> m_p_impl;
};

}

#endif /* IDLP_FILE_DEVICE_H */
