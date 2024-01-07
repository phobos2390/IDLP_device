/// @file IDLP/test/Fake_device_test.cpp

#include <IDLP/Fake_device.h>
#include <catch2/catch.hpp>
#include <cstring>
#include <thread>
#include <iostream>
#include <IDLP/Fake_device.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "Fake_device_test", "stack" )
{
    Fake_device c;
}

TEST_CASE( "Fake_device_test.basic_read_write", "Fake_device_test.basic_read_write" )
{
    uint8_t message[] = { 0x0, 0x1, 0x2, 0x3, 0x4 };
    uint8_t buffer[sizeof(message)];
    (void)std::memset(&buffer[0], 0, sizeof(buffer));
    Fake_device c;
    REQUIRE(static_cast<int64_t>(sizeof(message)) == c.write(&message[0], sizeof(message), 0));
    REQUIRE(static_cast<int64_t>(sizeof(buffer)) == c.read(&buffer[0], sizeof(buffer), 0));
    REQUIRE(0 == std::memcmp(&buffer[0], &message[0], sizeof(buffer)));
}

TEST_CASE( "Fake_device_test.read_timeout", "Fake_device_test.read_timeout" )
{
    const size_t buffer_size = 5;
    uint8_t buffer[buffer_size];
    (void)std::memset(&buffer[0], 0, sizeof(buffer));
    Fake_device c;
    REQUIRE(static_cast<int64_t>(IDLP::IDLP_ERROR_TIMEOUT) == c.read(&buffer[0], sizeof(buffer), 0));
}

TEST_CASE( "Fake_device_test.multithread_reader_writer", "Fake_device_test.multithread_reader_writer" )
{
    uint8_t message[] = {0x0,0x1,0x2,0x3,0x4};
    const size_t c_buffer_size = sizeof(message);
    uint8_t buffer[c_buffer_size];

    Fake_device c;
    IDLP::IDLP_Error writer_error = IDLP::IDLP_ERROR_NONE;
    IDLP::IDLP_Error reader_error = IDLP::IDLP_ERROR_NONE;
    std::thread writer([&message, &c, &writer_error]()
    {
        for(uint32_t i = 0; i < 100; i++)
        {
            IDLP::IDLP_Error current_error = c.write_exact(&message[0], sizeof(message), 1000);
            if(writer_error == IDLP::IDLP_ERROR_NONE)
            {
                writer_error == current_error;
            }
        }
    });
    std::thread reader([&buffer,&c,&reader_error]()
    {
        for(uint32_t i = 0; i < 100; i++)
        {
            IDLP::IDLP_Error current_error = c.read_exact(&buffer[0], sizeof(buffer), 1000);
            if(reader_error == IDLP::IDLP_ERROR_NONE)
            {
                reader_error == current_error;
            }
        }
    });
    reader.join();
    writer.join();

    REQUIRE(writer_error == IDLP_ERROR_NONE);
    REQUIRE(reader_error == IDLP_ERROR_NONE);
}