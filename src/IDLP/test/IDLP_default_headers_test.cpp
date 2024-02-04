/// @file IDLP/test/IDLP_default_headers_test.cpp

#include <IDLP/IDLP_default_headers.h>
#include <catch2/catch.hpp>
#include <cstring>
#include <IDLP/IDLP_endian.h>
#include <IDLP/IDLP_default_headers.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "IDLP_default_headers_test.stack", "IDLP_default_headers_test.stack" )
{
    IDLP::IDLP_message_header header;
    (void)memset(&header, 0, sizeof(header));
    IDLP::IDLP_element_header element_header;
    (void)memset(&element_header, 0, sizeof(element_header));
}

TEST_CASE( "IDLP_default_headers_test.ntoh", "IDLP_default_headers_test.ntoh" )
{
    IDLP::IDLP_message_header header;
    IDLP::IDLP_message_header header_network;
    IDLP::IDLP_message_header header_host;
    (void)memset(&header, 0, sizeof(header));
    (void)memset(&header_network, 0, sizeof(header_network));
    (void)memset(&header_host, 0, sizeof(header_host));

    header.m_crc = 0x01020304;
    header.m_header_type = static_cast<uint16_t>(IDLP_Header_Type_Command_Request);
    header.m_message_payload_size = 0x0506;

    (void)memcpy(&header_network, &header, sizeof(header));
    IDLP::hton(header_network);

    (void)memcpy(&header_host, &header_network, sizeof(header_network));
    IDLP::ntoh(header_host);
    REQUIRE(0 == memcmp(&header_host, &header, 0));
    REQUIRE(0x04030201 == header_network.m_crc);
    REQUIRE(hton(static_cast<uint16_t>(IDLP_Header_Type_Command_Request)) == header_network.m_header_type);
    REQUIRE(0x0605 == header_network.m_message_payload_size);

    IDLP::IDLP_element_header element_header;
    IDLP::IDLP_element_header element_header_network;
    IDLP::IDLP_element_header element_header_host;
    (void)memset(&element_header, 0, sizeof(element_header));
    (void)memset(&element_header_network, 0, sizeof(element_header_network));
    (void)memset(&element_header_host, 0, sizeof(element_header_host));

    element_header.m_element_id = 0x0708;
    element_header.m_element_payload_size = 0x090A;

    (void)memcpy(&element_header_network, &element_header, sizeof(element_header));
    IDLP::hton(element_header_network);

    (void)memcpy(&element_header_host, &element_header_network, sizeof(element_header_network));
    IDLP::ntoh(element_header_host);

    REQUIRE(0 == memcmp(&element_header_host, &element_header, 0));
    REQUIRE(0x0807 == element_header_network.m_element_id);
    REQUIRE(0x0A09 == element_header_network.m_element_payload_size);

}
