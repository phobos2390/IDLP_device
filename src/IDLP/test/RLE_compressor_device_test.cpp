/// @file IDLP/test/RLE_compressor_device_test.cpp

#include <IDLP/RLE_compressor_device.h>
#include <catch2/catch.hpp>
#include <IDLP/RLE_compressor_device.h> // Testing include guard
#include <IDLP/Fake_device.h>
#include <cstring>
#include <iostream>

using namespace IDLP;

TEST_CASE( "RLE_compressor_device_test.stack", "RLE_compressor_device_test.stack" )
{
    Fake_device f;
    RLE_compressor_device c(f);
}

IDLP::IDLP_Error test_input (uint8_t* input_message, size_t input_size, uint8_t* expected_output, size_t expected_output_size)
{
    Fake_device f;
    RLE_compressor_device rle_device(f);
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;
    uint8_t actual_output [expected_output_size];
    uint8_t actual_uncompressed_output [input_size];
    (void) std::memset (&actual_output, 0, sizeof(actual_output));
    (void) std::memset (&actual_uncompressed_output, 0, sizeof(actual_uncompressed_output));
    size_t actual_output_size = rle_device.write (&input_message[0], input_size, 0);
    if (expected_output_size == actual_output_size)
    {
        error = f.read_exact (&actual_output[0], expected_output_size, 0);
    }
    else
    {
        error = IDLP::IDLP_ERROR_BAD_SIZE;
    }
    if ((error == IDLP::IDLP_ERROR_NONE) && (0 != std::memcmp (&actual_output[0], &expected_output[0], expected_output_size)))
    {
        error = IDLP::IDLP_ERROR_INVALID;
    }
    if (error == IDLP::IDLP_ERROR_NONE)
    {
        error = f.write_exact(&actual_output[0],expected_output_size,0);
    }
    if (error == IDLP::IDLP_ERROR_NONE)
    {
        error = rle_device.read_exact(&actual_uncompressed_output[0],input_size,0);
    }
    if ((error == IDLP::IDLP_ERROR_NONE) && (0 != std::memcmp (&input_message[0], &actual_uncompressed_output[0], input_size)))
    {
        error = IDLP::IDLP_ERROR_CORRUPTION;
    }
    return error; 
}

TEST_CASE( "RLE_compressor_device_test.compress", "RLE_compressor_device_test.compress" )
{
    uint8_t input_message [] = { 0x0, 0x0, 0x0, 0x0 };
    uint8_t output_message [] = { 0x0, 0xFF, 0x4  };
    REQUIRE (IDLP::IDLP_ERROR_NONE == test_input (&input_message[0], sizeof(input_message), &output_message[0], sizeof(output_message)));
}

TEST_CASE( "RLE_compressor_device_test.compress2", "RLE_compressor_device_test.compress2" )
{
    uint8_t input_message [] = { 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 };
    uint8_t output_message [] = { 0x0, 0xFF, 0x4, 0x1, 0xFF, 0x8 };
    REQUIRE (IDLP::IDLP_ERROR_NONE == test_input (&input_message[0], sizeof(input_message), &output_message[0], sizeof(output_message)));
}

TEST_CASE( "RLE_compressor_device_test.escaping", "RLE_compressor_device_test.escaping" )
{
    uint8_t input_message [] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t output_message [] = { 0xFF, 0xFF, 0xFF, 0x6 };
    REQUIRE (IDLP::IDLP_ERROR_NONE == test_input (&input_message[0], sizeof(input_message), &output_message[0], sizeof(output_message)));
}

TEST_CASE( "RLE_compressor_device_test.size_too_big", "RLE_compressor_device_test.size_too_big" )
{
    uint8_t input_message [ 0xFF ];
    (void) std::memset(&input_message[0], 0, sizeof(input_message));
    
    uint8_t output_message [] = { 0x00, 0xFF, 0xFE, 0x0 };
    REQUIRE (IDLP::IDLP_ERROR_NONE == test_input (&input_message[0], sizeof(input_message), &output_message[0], sizeof(output_message)));
}

TEST_CASE( "RLE_compressor_device_test.size_too_big_escaped", "RLE_compressor_device_test.size_too_big_escaped" )
{
    uint8_t input_message [ 0x100 ];
    (void) std::memset(&input_message[0], 0xFF, sizeof(input_message));
    
    uint8_t output_message [] = { 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x2 };
    REQUIRE (IDLP::IDLP_ERROR_NONE == test_input (&input_message[0], sizeof(input_message), &output_message[0], sizeof(output_message)));
}

TEST_CASE( "RLE_compressor_device_test.nonescaped", "RLE_compressor_device_test.nonescaped" )
{ 
    uint8_t input_message [] = { 0, 1, 5, 2, 3, 2, 1 };
    uint8_t output_message [] = { 0, 1, 5, 2, 3, 2, 1 };
    REQUIRE (IDLP::IDLP_ERROR_NONE == test_input (&input_message[0], sizeof(input_message), &output_message[0], sizeof(output_message)));
}
