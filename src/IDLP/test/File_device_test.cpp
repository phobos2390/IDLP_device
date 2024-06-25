/// @file IDLP/test/File_device_test.cpp

#include <IDLP/File_device.h>
#include <catch2/catch.hpp>
#include <IDLP/File_device.h> // Testing include guard
#include <cstring>
#include <fcntl.h>

using namespace IDLP;

TEST_CASE( "File_device_test.stack", "File_device_test.stack" )
{
    File_device c;
}

TEST_CASE( "File_device_test.basic_file_write_test", "File_device_test.basic_file_write_test" )
{
    File_device c;
    uint8_t message [] = { 0, 0, 0 };
    REQUIRE(IDLP::IDLP_ERROR_NONE == c.open_file("sample_file.txt", (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)));
    
    REQUIRE(static_cast<int32_t>(sizeof(message)) == c.write(&message[0], sizeof(message), 0));
}

TEST_CASE( "File_device_test.basic_file_write_read_test", "File_device_test.basic_file_write_read_test" )
{
    const char* message = "Lorem ipsum" ;
    size_t message_length = strlen(message) + 1;
    uint8_t out_buffer [0x100];
    uint8_t in_buffer [0x100];
    (void)std::memset(&out_buffer[0], 0, sizeof(out_buffer));
    (void)std::memset(&in_buffer[0],  0, sizeof(in_buffer));
    (void)std::memcpy(&out_buffer[0],&message[0], message_length);
   
    { 
        File_device f;
        REQUIRE(IDLP::IDLP_ERROR_NONE == f.open_file("sample_write_read.txt", (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)));
        
        REQUIRE(static_cast<int32_t>(message_length) == f.write(&out_buffer[0], message_length, 0));
    }
    {
        File_device f;
        REQUIRE(IDLP::IDLP_ERROR_NONE == f.open_file("sample_write_read.txt", (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)));
        REQUIRE(static_cast<int32_t>(message_length) == f.read(&in_buffer[0],   message_length, 0));
    }
    REQUIRE(0 == std::memcmp(&in_buffer[0], &out_buffer[0], message_length));
   
}
