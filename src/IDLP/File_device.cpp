/// @file IDLP/File_device.cpp

#include <IDLP/File_device.h>
#include <memory>
#include <mutex>
#include <string>

///Linux
#include <unistd.h>
#include <fcntl.h>

namespace IDLP
{

ssize_t fd_write(int32_t fd, uint8_t* out_bytes, size_t bytes_size)
{
    return write(fd, out_bytes, bytes_size);
}

ssize_t fd_read(int32_t fd, uint8_t* in_bytes, size_t max_bytes_size)
{
    return read(fd, in_bytes, max_bytes_size);
}

struct File_device::Impl
{
public:
    int32_t m_file_descriptor;
    std::timed_mutex m_mutex;
    
    Impl()
       :m_file_descriptor(0)
       ,m_mutex()
    {
    }
    virtual ~Impl()
    {
        if(m_file_descriptor > 0)
        {
            close(m_file_descriptor);
        }
    }

    int64_t write_bytes(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms)
    {
        IDLP::IDLP_Error error = IDLP::IDLP_ERROR_TIMEOUT;
        int64_t bytes_written = 0;
        ssize_t result = 0;
      
        if(m_mutex.try_lock_for(std::chrono::milliseconds(timeout_ms)))
        {
            result = fd_write(m_file_descriptor, out_bytes, bytes_size);
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
            m_mutex.unlock();
        }
        return bytes_written;
    }
    
    int64_t read_bytes(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)
    {
        ssize_t result = 0;
        IDLP::IDLP_Error error = IDLP::IDLP_ERROR_TIMEOUT;
 
        if(m_mutex.try_lock_for(std::chrono::milliseconds(timeout_ms)))
        {
            result = fd_read(m_file_descriptor, in_bytes, max_bytes_size);
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
            m_mutex.unlock();
        }
        return result;
    }
};

File_device::File_device()
    :m_p_impl(std::make_unique<Impl>())
{
}

File_device::~File_device()
{
}

int64_t File_device::write(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms)
{
    return m_p_impl->write_bytes(out_bytes,bytes_size,timeout_ms);
}

int64_t File_device::read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)
{
    return m_p_impl->read_bytes(in_bytes,max_bytes_size,timeout_ms);
}

IDLP::IDLP_Error File_device::open_file(std::string filename, int32_t flags, int32_t mode)
{
    IDLP::IDLP_Error error = IDLP::IDLP_ERROR_FILE;
    m_p_impl->m_file_descriptor = open(filename.c_str(), flags, mode);
    if(m_p_impl->m_file_descriptor > 0)
    {
        error = IDLP::IDLP_ERROR_NONE;
    }
    else
    {
        error = -errno;
    }
    return error;
}

}
