/// @file IDLP/test/IDLP_CRC_16_test.cpp

#include <IDLP/IDLP_CRC_16.h>
#include <catch2/catch.hpp>
#include <IDLP/IDLP_CRC_16.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "IDLP_CRC_16_test", "stack" )
{
    REQUIRE(0 == IDLP::crc_16(0, 0));
}
