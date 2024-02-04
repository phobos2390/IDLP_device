/// @file IDLP/test/IDLP_CRC_32_test.cpp

#include <IDLP/IDLP_CRC_32.h>
#include <catch2/catch.hpp>
#include <IDLP/IDLP_CRC_32.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "IDLP_CRC_32_test", "stack" )
{
    REQUIRE(0 == IDLP::crc_32(0, 0));
}
