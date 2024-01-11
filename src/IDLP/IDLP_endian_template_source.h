/// @file IDLP/IDLP_endian_template_source.h

#include <IDLP/IDLP_endian.h>
#include <cstdint>

#ifndef IDLP_IDLP_ENDIAN_TEMPLATE_SOURCE_H
#define IDLP_IDLP_ENDIAN_TEMPLATE_SOURCE_H

namespace IDLP
{
template<typename T>
T hton(const T& input)
{
    T retval = 0;
    uint8_t* p_retval = reinterpret_cast<uint8_t*>(&retval);
    for(std::size_t i = 0; i < sizeof(T); i++)
    {
        T mask = 0xFF;
        std::size_t shift_amount = (sizeof(T) - (i + 1)) * 8;
        mask = mask << shift_amount;
        p_retval[i] = ((input & mask) >> shift_amount);
    }
    
    return retval;
}

template<typename T>
T ntoh(const T& input)
{
    T retval = 0;
    const uint8_t* p_input = reinterpret_cast<const uint8_t*>(&input);
    for(std::size_t i = 0; i < sizeof(T); i++)
    {
        retval = retval << 8;
        retval |= static_cast<T>(p_input[i]);
    }
    
    return retval;
}
}
#endif /*IDLP_IDLP_ENDIAN_TEMPLATE_SOURCE_H*/