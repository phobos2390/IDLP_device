/// @file IDLP/IDLP_Errors.h

#ifndef IDLP_IDLP_ERRORS_H
#define IDLP_IDLP_ERRORS_H

#include <cstdint>

namespace IDLP
{
    typedef int32_t IDLP_Error;

    const static int32_t IDLP_ERROR_NONE = 0;
    const static int32_t IDLP_ERROR_NULL = 0;
    const static int32_t IDLP_ERROR_INVALID = -2;
    const static int32_t IDLP_ERROR_TIMEOUT = -3;
    const static int32_t IDLP_ERROR_NOT_FOUND = -4;
}

#endif /* IDLP_IDLP_ERRORS_H */
