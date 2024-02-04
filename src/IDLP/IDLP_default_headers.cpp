/// @file IDLP/IDLP_default_headers.cpp

#include <IDLP/IDLP_default_headers.h>
#include <IDLP/IDLP_endian.h>

namespace IDLP
{

void ntoh(IDLP_message_header& header)
{
    header.m_header_type = ntoh(header.m_header_type);
    header.m_message_payload_size = ntoh(header.m_message_payload_size);
    header.m_crc = ntoh(header.m_crc);
}
void ntoh(IDLP_element_header& header)
{
    header.m_element_id = ntoh(header.m_element_id);
    header.m_element_payload_size = ntoh(header.m_element_payload_size);
}

void hton(IDLP_message_header& header)
{
    header.m_header_type = hton(header.m_header_type);
    header.m_message_payload_size = hton(header.m_message_payload_size);
    header.m_crc = hton(header.m_crc);
}
void hton(IDLP_element_header& header)
{
    header.m_element_id = hton(header.m_element_id);
    header.m_element_payload_size = hton(header.m_element_payload_size);
}


}
