/// @file IDLP/IDLP_CRC_16.h

#ifndef IDLP_IDLP_CRC_16_H
#define IDLP_IDLP_CRC_16_H

#include <cstdint>
#include <cstdlib>

namespace IDLP
{

/// @brief 
/// @param array 
/// @param array_size 
/// @return 
uint16_t crc_16(uint8_t* array, size_t array_size);

/// @brief 
/// @param array 
/// @param array_size 
/// @param table 
/// @return 
uint16_t crc_16(uint8_t* array, size_t array_size, const uint16_t (&table)[0x100]);

/// @brief 
/// @param table 
/// @param polynomial 
void generate_crc_16_table(uint16_t (&table)[0x100], uint16_t polynomial);
}

#endif /* IDLP_IDLP_CRC_16_H */
