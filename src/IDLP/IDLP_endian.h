/// @file IDLP/IDLP_endian.h

#ifndef IDLP_IDLP_ENDIAN_H
#define IDLP_IDLP_ENDIAN_H

namespace IDLP
{
    /// @brief Generic hton
    /// @tparam T type to put 
    /// @param[in] input - incoming network value
    /// @return host value
    template<typename T>
    T ntoh(const T& input);

    /// @brief Generic hton
    /// @tparam T type to put 
    /// @param[in] input - incoming host value
    /// @return network value
    template<typename T>
    T hton(const T& input);
}

#include <IDLP/IDLP_endian_template_source.h>

#endif /* IDLP_IDLP_ENDIAN_H */
