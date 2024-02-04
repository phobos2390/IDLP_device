/// @file IDLP/IDLP_default_headers.h

#ifndef IDLP_IDLP_DEFAULT_HEADERS_H
#define IDLP_IDLP_DEFAULT_HEADERS_H

#include <cstdint>

namespace IDLP
{

enum IDLP_Header_Type
{
    IDLP_Header_Type_Ping,
    IDLP_Header_Type_Command_Request,
    IDLP_Header_Type_Command_Response,
    IDLP_Header_Type_Status_Request,
    IDLP_Header_Type_Status_Response
};

struct IDLP_message_header
{
    uint16_t m_header_type;
    uint16_t m_message_payload_size;
    uint32_t m_crc;
}__attribute__((packed));

struct IDLP_element_header
{
    uint16_t m_element_id;
    uint16_t m_element_payload_size;
}__attribute__((packed));

/// @brief 
/// @param header 
void ntoh(IDLP_message_header& header);

/// @brief 
/// @param header 
void ntoh(IDLP_element_header& header);

/// @brief 
/// @param header 
void hton(IDLP_message_header& header);

/// @brief 
/// @param header 
void hton(IDLP_element_header& header);

}

#endif /* IDLP_IDLP_DEFAULT_HEADERS_H */
