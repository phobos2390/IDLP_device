/// @file IDLP/I_device.cpp

#include <IDLP/I_device.h>

namespace IDLP
{

IDLP::IDLP_Error I_device::write_exact(uint8_t* out_bytes, size_t expected_bytes_size, size_t timeout_ms)
{
    int64_t result = write(out_bytes, expected_bytes_size, timeout_ms);
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;
    if(result < 0)
    {
        error = static_cast<IDLP::IDLP_Error>(result);
    }
    else if(result != expected_bytes_size)
    {
        error = IDLP::IDLP_ERROR_INVALID;
    }
    return error;
}

IDLP::IDLP_Error I_device::read_exact(uint8_t* in_bytes, size_t expected_bytes_size, size_t timeout_ms)
{
    int64_t result = read(in_bytes, expected_bytes_size, timeout_ms);
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;
    if(result < 0)
    {
        error = static_cast<IDLP::IDLP_Error>(result);
    }
    else if(result != expected_bytes_size)
    {
        error = IDLP::IDLP_ERROR_INVALID;
    }
    return error;
}

}
