/// @file IDLP/test/IDLP_endian_test.cpp

#include <IDLP/IDLP_endian.h>
#include <catch2/catch.hpp>
#include <cstring>
#include <IDLP/IDLP_endian.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "IDLP_endian_test.basic", "IDLP_endian_test.basic" )
{
    uint16_t host_val_16 = 0x0123;
    uint32_t host_val_32 = 0x01234567;
    uint64_t host_val_64 = 0x0123456789ABCDEF;
    uint8_t network_val_arr_16[] = { 0x01, 0x23 };
    uint8_t network_val_arr_32[] = { 0x01, 0x23, 0x45, 0x67 };
    uint8_t network_val_arr_64[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    uint16_t network_val_16 = IDLP::hton(host_val_16);
    uint32_t network_val_32 = IDLP::hton(host_val_32);
    uint64_t network_val_64 = IDLP::hton(host_val_64);
    REQUIRE(0 == std::memcmp(reinterpret_cast<uint8_t*>(&network_val_16),&network_val_arr_16[0], sizeof(network_val_16)));
    REQUIRE(0 == std::memcmp(reinterpret_cast<uint8_t*>(&network_val_32),&network_val_arr_32[0], sizeof(network_val_32)));
    REQUIRE(0 == std::memcmp(reinterpret_cast<uint8_t*>(&network_val_64),&network_val_arr_64[0], sizeof(network_val_64)));
    REQUIRE(IDLP::ntoh(network_val_16) == host_val_16);
    REQUIRE(IDLP::ntoh(network_val_32) == host_val_32);
    REQUIRE(IDLP::ntoh(network_val_64) == host_val_64);
}
