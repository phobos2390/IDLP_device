/// @file IDLP/test/UDP_device_test.cpp

#include <IDLP/UDP_device.h>
#include <catch2/catch.hpp>
#include <thread>
#include <IDLP/UDP_device.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "UDP_device_test.stack", "UDP_device_test.stack" )
{
    UDP_device c;
}

TEST_CASE( "UDP_device_test.udp_test", "UDP_device_test.udp_test" )
{
    UDP_device client;
    UDP_device server;
    uint16_t port = 8000;
    uint16_t max_port = 8100;
    uint32_t localhost = 0x7F000001;
    uint16_t attempt_port = port;
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;
    bool found_port = false;
    do
    {
        error = server.set_receiving_port(attempt_port);
        if(error == IDLP::IDLP_ERROR_NONE)
        {
            found_port = true;
            port = attempt_port;
        }
        else
        {
            REQUIRE(error == -EADDRINUSE);
            attempt_port++;
            REQUIRE(attempt_port < max_port);
        }
    }
    while(!found_port);
    REQUIRE(IDLP::IDLP_ERROR_NONE == client.set_sending_port(port, localhost));

    uint8_t message[] = {0x0,0x1,0x2,0x3,0x4};
    const size_t c_buffer_size = sizeof(message);
    uint8_t buffer[c_buffer_size];


    IDLP::IDLP_Error writer_error = IDLP::IDLP_ERROR_NONE;
    IDLP::IDLP_Error reader_error = IDLP::IDLP_ERROR_NONE;
    std::thread writer([&message, &client, &writer_error]()
    {
        for(uint32_t i = 0; i < 10; i++)
        {
            IDLP::IDLP_Error current_error = client.write_exact(&message[0], sizeof(message), 1000);
            if(writer_error == IDLP::IDLP_ERROR_NONE)
            {
                writer_error == current_error;
            }
        }
    });
    std::thread reader([&buffer,&server,&reader_error]()
    {
        for(uint32_t i = 0; i < 10; i++)
        {
            IDLP::IDLP_Error current_error = server.read_exact(&buffer[0], sizeof(buffer), 1000);
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


TEST_CASE( "UDP_device_test.server_conflict", "UDP_device_test.server_conflict" )
{
    UDP_device colliding_server;
    UDP_device server;
    uint16_t colliding_port = 8000;
    uint16_t port = 8000;
    uint16_t max_port = 8100;
    uint16_t attempt_port = port;
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;

    // attempt to find port
    bool found_port = false;
    do
    {
        error = colliding_server.set_receiving_port(attempt_port);
        if(error == IDLP::IDLP_ERROR_NONE)
        {
            found_port = true;
            colliding_port = attempt_port;
        }
        else
        {
            REQUIRE(error == -EADDRINUSE);
            attempt_port++;
            REQUIRE(attempt_port < max_port);
        }
    }
    while(!found_port);
    
    // verify that a second port can be found
    found_port = false;
    do
    {
        error = server.set_receiving_port(attempt_port);
        if(error == IDLP::IDLP_ERROR_NONE)
        {
            found_port = true;
            port = attempt_port;
        }
        else
        {
            REQUIRE(error == -EADDRINUSE);
            attempt_port++;
            REQUIRE(attempt_port < max_port);
        }
    }
    while(!found_port);
    REQUIRE(colliding_port != port);
}
