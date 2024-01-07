/// @file IDLP/UDP_device.cpp

#include <IDLP/UDP_device.h>

//#ifdef LINUX
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <errno.h>
#include <cstring>


namespace IDLP
{

struct UDP_device::Impl
{
public:
    int32_t m_socket_file_descriptor;
    sockaddr m_receiving_address;
    sockaddr m_sending_address;

    Impl()
        :m_socket_file_descriptor(0)
        ,m_receiving_address()
        ,m_sending_address()
    {
        (void)std::memset(&m_receiving_address, 0, sizeof(m_receiving_address));
        (void)std::memset(&m_sending_address, 0, sizeof(m_sending_address));
    }

    virtual ~Impl()
    {
        if(m_socket_file_descriptor > 0)
        {
            close(m_socket_file_descriptor);
        }
    }

    IDLP::IDLP_Error wait_for_fd(size_t timeout_ms)
    {
        IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;
        int32_t select_result = 0;

        timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        select_result = select(m_socket_file_descriptor, 0, 0, 0, &tv);

        if(select_result < 0)
        {
            error = static_cast<IDLP::IDLP_Error>(-errno);
        }
        else if(select_result == 0)
        {
            error = IDLP::IDLP_ERROR_TIMEOUT;
        }

        return error;
    }
};

UDP_device::UDP_device()
    :m_p_impl(new Impl)
{
}

UDP_device::~UDP_device()
{
    delete m_p_impl;
    m_p_impl = 0;
}


int64_t UDP_device::write(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms)
{
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;
    int64_t bytes_written = 0;

    error = m_p_impl->wait_for_fd(timeout_ms);
    if(error == IDLP::IDLP_ERROR_NONE)
    {
        ssize_t result = sendto(m_p_impl->m_socket_file_descriptor, 
                                out_bytes,
                                bytes_size,
                                0,
                                &m_p_impl->m_receiving_address, 
                                sizeof(m_p_impl->m_receiving_address)); 
        if(result < 0)
        {
            result = static_cast<IDLP::IDLP_Error>(-errno);
        }
        else
        {
            bytes_written = static_cast<int64_t>(result);
        }
    }
    else
    {
        bytes_written = static_cast<uint64_t>(error);
    }
    return bytes_written;
}

int64_t UDP_device::read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)
{
    socklen_t sock_struct_length = sizeof(m_p_impl->m_sending_address);
    ssize_t result = 0;
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NONE;

    error = m_p_impl->wait_for_fd(timeout_ms);
    if(error == IDLP::IDLP_ERROR_NONE)
    {
        result = recvfrom(m_p_impl->m_socket_file_descriptor, 
                          in_bytes, 
                          max_bytes_size,  
                          MSG_DONTWAIT, 
                          &m_p_impl->m_sending_address, 
                          &sock_struct_length);
    }
    else 
    {
        result = static_cast<int64_t>(error);
    }

    return result;
}

IDLP::IDLP_Error UDP_device::set_receiving_port(uint16_t port)
{
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_INVALID;
    if(m_p_impl->m_socket_file_descriptor == 0)
    {
        // Filling server information 
        sockaddr_in new_addr;
        (void)std::memset(&new_addr, 0, sizeof(new_addr));

        new_addr.sin_family      = AF_INET; // IPv4 
        new_addr.sin_addr.s_addr = INADDR_ANY; 
        new_addr.sin_port        = port; 

        (void)std::memcpy(&m_p_impl->m_receiving_address, &new_addr, sizeof(new_addr));

        int32_t fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd >= 0)
        {
            int32_t bind_result = bind(fd, &m_p_impl->m_receiving_address, sizeof(sockaddr));
            if(bind_result < 0)
            {
                (void)close(fd);
                error = static_cast<IDLP::IDLP_Error>(-errno);
            }
            else
            {
                m_p_impl->m_socket_file_descriptor = fd;
                error = IDLP::IDLP_ERROR_NONE;
            }
        }
        else
        {
            error = static_cast<IDLP::IDLP_Error>(fd);
        }
    }
    return error;
}

IDLP::IDLP_Error UDP_device::set_sending_port(uint16_t port, uint32_t ipv4_address)
{
    sockaddr_in new_addr;
    (void)std::memset(&new_addr, 0, sizeof(new_addr));

    new_addr.sin_family      = AF_INET; // IPv4 
    new_addr.sin_addr.s_addr = ipv4_address; 
    new_addr.sin_port        = port;

    (void)std::memcpy(&m_p_impl->m_sending_address, &new_addr, sizeof(new_addr));

    return IDLP::IDLP_ERROR_NONE;
}


}
//#else
//#error "not set up to use non Linux libraries for sockets"
//#endif