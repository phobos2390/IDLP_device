/// @file IDLP/test/UDP_device_test.cpp

#include <IDLP/UDP_device.h>
#include <catch2/catch.hpp>
#include <thread>
#include <iostream>
#include <sstream>
#include <cstring>
#include <IDLP/UDP_device.h> // Testing include guard

using namespace IDLP;

TEST_CASE( "UDP_device_test.stack", "UDP_device_test.stack" )
{
    UDP_device c;
}

TEST_CASE( "UDP_device_test.simple_udp_test", "UDP_device_test.simple_udp_test" )
{
    UDP_device client;
    UDP_device server;
    uint16_t port = 8000;
    uint16_t max_port = 8100;
    uint32_t localhost = 0x00000000;
    uint16_t attempt_port = port;
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;

    REQUIRE(IDLP::IDLP_ERROR_NONE == server.find_open_port(port, port, max_port));
    REQUIRE(IDLP::IDLP_ERROR_NONE == client.set_sending_port(port, localhost));

    uint8_t message[] = {0x0,0x1,0x2,0x3,0x4};
    const size_t c_buffer_size = sizeof(message);
    uint8_t buffer[c_buffer_size];

    REQUIRE(IDLP::IDLP_ERROR_NONE == client.write_exact(&message[0], sizeof(message), 10000));
    REQUIRE(IDLP::IDLP_ERROR_NONE == server.read_exact(&buffer[0], sizeof(buffer), 10000));
    REQUIRE(std::memcmp(&buffer[0],&message[0],sizeof(buffer)) == 0);
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

    REQUIRE(IDLP::IDLP_ERROR_NONE == server.find_open_port(port, port, max_port));
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
                writer_error = current_error;
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
                reader_error = current_error;
            }
        }
    });
    reader.join();
    writer.join();

    REQUIRE(writer_error == IDLP_ERROR_NONE);
    REQUIRE(reader_error == IDLP_ERROR_NONE);
    REQUIRE(std::memcmp(&buffer[0],&message[0],sizeof(buffer)) == 0);
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

    REQUIRE(IDLP::IDLP_ERROR_NONE == colliding_server.find_open_port(colliding_port,port,max_port));
    REQUIRE(IDLP::IDLP_ERROR_NONE == server.find_open_port(port,port,max_port));
    REQUIRE(colliding_port != port);
}

TEST_CASE( "UDP_device_test.udp_spiral_handoff_test", "UDP_device_test.udp_spiral_handoff_test" )
{
    UDP_device client;
    UDP_device server;
    uint16_t port = 8000;
    uint16_t max_port = 8100;
    uint32_t localhost = 0x7F000001;
    uint16_t attempt_port = port;
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;

    REQUIRE(IDLP::IDLP_ERROR_NONE == server.find_open_port(port, port, max_port));
    REQUIRE(IDLP::IDLP_ERROR_NONE == client.set_sending_port(port, localhost));

    IDLP::IDLP_Error writer_error = IDLP::IDLP_ERROR_NONE;
    IDLP::IDLP_Error reader_error = IDLP::IDLP_ERROR_NONE;
    std::thread writer([&client, &writer_error]()
    {
        std::stringstream error_message;
        uint64_t spiral_client = 0;
        uint64_t spiral_client_received = 0;
        for(uint32_t i = 0; i < 2; i++)
        {
            std::cout << "Sending spiral client message \n" << std::endl;
            IDLP::IDLP_Error current_error = client.write_exact(reinterpret_cast<uint8_t*>(&spiral_client), 
                                                                sizeof(spiral_client), 
                                                                10000);
            if(writer_error == IDLP::IDLP_ERROR_NONE)
            {
                writer_error = current_error;
            }
            if(current_error != IDLP::IDLP_ERROR_NONE)
            {
                error_message.str("");
                error_message << "There was an error writing the spiral client message " << current_error << std::endl;
                std::cout << error_message.str();
            }
            current_error = client.read_exact(reinterpret_cast<uint8_t*>(&spiral_client_received), 
                                              sizeof(spiral_client_received), 
                                              10000);
            if(writer_error == IDLP::IDLP_ERROR_NONE)
            {
                writer_error = current_error;
            }
            if(current_error != IDLP::IDLP_ERROR_NONE)
            {
                error_message.str("");
                error_message << "There was an error reading the spiral client response " << current_error << std::endl;
                std::cout << error_message.str();
            }
            if(spiral_client + 1 != spiral_client_received)
            {
                error_message.str("");
                error_message << "Failed spiral client message: " 
                              << (spiral_client + 1) 
                              << " != " << spiral_client_received 
                              << std::endl;
                std::cout << error_message.str();
            }
            else
            {
                std::cout << "Successfully received spiral client message \n" << std::endl;
            }
            spiral_client++;
        }
    });
    std::thread reader([&server,&reader_error]()
    {
        std::stringstream error_message;
        uint64_t spiral_server_received = 0;
        uint64_t spiral_server = 0;
        for(uint32_t i = 0; i < 2; i++)
        {
            IDLP::IDLP_Error current_error = server.read_exact(reinterpret_cast<uint8_t*>(&spiral_server_received), 
                                                               sizeof(spiral_server_received), 
                                                               10000);

            if(current_error != IDLP::IDLP_ERROR_NONE)
            {
                error_message.str("");
                error_message << "There was an error reading the spiral serverd " << current_error << std::endl;
                std::cout << error_message.str();
            }
            else
            {
                error_message.str("");
                error_message << "Read spiral server message: " << spiral_server_received << std::endl;
                std::cout << error_message.str();
            }
            if(reader_error == IDLP::IDLP_ERROR_NONE)
            {
                reader_error = current_error;
            }
            spiral_server = spiral_server_received + 1;
            current_error = server.write_exact(reinterpret_cast<uint8_t*>(&spiral_server), 
                                               sizeof(spiral_server), 
                                               10000);
            std::cout << "Wrote spiral server message \n" << std::endl;
            if(reader_error == IDLP::IDLP_ERROR_NONE)
            {
                reader_error = current_error;
            }
        }
    });
    reader.join();
    writer.join();

    REQUIRE(writer_error == IDLP_ERROR_NONE);
    REQUIRE(reader_error == IDLP_ERROR_NONE);
}

