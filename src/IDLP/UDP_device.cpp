/// @file IDLP/UDP_device.cpp

#include <IDLP/UDP_device.h>
#include <mutex>

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
#include <IDLP/IDLP_endian.h>

namespace IDLP
{

struct UDP_device::Impl
{
public:
    int32_t m_socket_file_descriptor;
    sockaddr m_receiving_address;
    sockaddr m_sending_address;
    std::timed_mutex m_mutex;

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

        fd_set rfds;
        // fd_set wfds;

        FD_ZERO(&rfds);
        FD_SET(m_socket_file_descriptor, &rfds);

        // FD_ZERO(&wfds);
        // FD_SET(m_socket_file_descriptor, &wfds);

        select_result = setsockopt(m_socket_file_descriptor, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
//        select_result = select(1, &rfds, 0, 0, &tv);

        if(select_result < 0)
        {
            if(errno == ETIMEDOUT)
            {
                error = IDLP::IDLP_ERROR_TIMEOUT;
            }
            else
            {
                error = static_cast<IDLP::IDLP_Error>(-errno);
            }
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
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_TIMEOUT;
    int64_t bytes_written = 0;

    if(m_p_impl->m_mutex.try_lock_for(std::chrono::milliseconds(timeout_ms)))
    {
        ssize_t result = sendto(m_p_impl->m_socket_file_descriptor, 
                                out_bytes,
                                bytes_size,
                                0,
                                &m_p_impl->m_sending_address, 
                                sizeof(m_p_impl->m_sending_address)); 
        if(result < 0)
        {
            if(errno == ETIMEDOUT)
            {
                bytes_written = IDLP::IDLP_ERROR_TIMEOUT;
            }
            else
            {
                bytes_written = static_cast<IDLP::IDLP_Error>(-errno);
            }
        }
        else
        {
            bytes_written = static_cast<int64_t>(result);
        }
        m_p_impl->m_mutex.unlock();
    }
    return bytes_written;
}

int64_t UDP_device::read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)
{
    socklen_t sock_struct_length = sizeof(m_p_impl->m_sending_address);
    ssize_t result = 0;
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_TIMEOUT;

    if(m_p_impl->m_mutex.try_lock_for(std::chrono::milliseconds(timeout_ms)))
    {
        error = m_p_impl->wait_for_fd(timeout_ms);
        if(error == IDLP::IDLP_ERROR_NONE)
        {
            result = recvfrom(m_p_impl->m_socket_file_descriptor, 
                            in_bytes, 
                            max_bytes_size,  
                            0, 
                            &m_p_impl->m_sending_address, 
                            &sock_struct_length);
            if(result < 0)
            {
                if(errno == ETIMEDOUT)
                {
                    result = static_cast<int64_t>(IDLP::IDLP_ERROR_TIMEOUT);
                }
                else
                {
                    result = static_cast<int64_t>(-errno);
                }
            }
        }
        else 
        {
            result = static_cast<int64_t>(error);
        }
        m_p_impl->m_mutex.unlock();
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
        new_addr.sin_port        = IDLP::hton(port); 

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
            error = static_cast<IDLP::IDLP_Error>(-errno);
        }
    }
    return error;
}

IDLP::IDLP_Error UDP_device::set_sending_port(uint16_t port, uint32_t ipv4_address)
{
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_INVALID;
    if(m_p_impl->m_socket_file_descriptor == 0)
    {
        int32_t fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd >= 0)
        {
            sockaddr_in new_addr;
            (void)std::memset(&new_addr, 0, sizeof(new_addr));

            new_addr.sin_family      = AF_INET; // IPv4 
            new_addr.sin_addr.s_addr = IDLP::hton(ipv4_address); 
            new_addr.sin_port        = IDLP::hton(port);

            (void)std::memcpy(&m_p_impl->m_sending_address, &new_addr, sizeof(new_addr));
            m_p_impl->m_socket_file_descriptor = fd;
            error = IDLP::IDLP_ERROR_NONE;
        }
        else
        {
            error = static_cast<IDLP::IDLP_Error>(-errno);
        }
    }

    return error;
}

IDLP::IDLP_Error UDP_device::find_open_port(uint16_t& port, uint16_t min_port, uint16_t max_port)
{
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_NOT_FOUND;
    IDLP::IDLP_Error current_error = IDLP::IDLP_ERROR_NOT_FOUND;
    bool continuing = true;
    port = min_port;
    do
    {
        current_error = set_receiving_port(port);
        if(current_error == IDLP::IDLP_ERROR_NONE)
        {
            continuing = false;
            error = current_error;
        }
        else if(current_error != static_cast<IDLP::IDLP_Error>(-EADDRINUSE))
        {
            continuing = false;
            error = current_error;
        }
        else if(port < max_port)
        {
            port++;
        }
        else
        {
            continuing = false;
        }
    } 
    while (continuing);
    
    return error;
}

}
//#else
//#error "not set up to use non Linux libraries for sockets"
//#endif