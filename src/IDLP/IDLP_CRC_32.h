/// @file IDLP/IDLP_CRC_32.h

#ifndef IDLP_IDLP_CRC_32_H
#define IDLP_IDLP_CRC_32_H

#include <cstdint>
#include <cstdlib>

namespace IDLP
{

/// @brief 
/// @param array 
/// @param array_size 
/// @return 
uint32_t crc_32(uint8_t* array, size_t array_size);

/// @brief 
/// @param array 
/// @param array_size 
/// @param table 
/// @return 
uint32_t crc_32(uint8_t* array, size_t array_size, const uint32_t (&table)[0x100]);

/// @brief 
/// @param[out] table 
/// @param[in] polynomial 
void generate_crc_32_table(uint32_t (&table)[0x100], uint32_t polynomial);

}

#endif /* IDLP_IDLP_CRC_32_H */
