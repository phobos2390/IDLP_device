/// @file IDLP/Fake_device.cpp

#include <IDLP/Fake_device.h>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <IDLP/IDLP_Semaphore.h>
#include <semaphore>

namespace IDLP
{

struct Fake_device::Impl
{
public:
    std::queue<std::vector<uint8_t>> m_packet_queue;
    std::timed_mutex m_queue_mutex;
    IDLP::IDLP_Semaphore m_queue_not_empty;

    Impl()
        :m_packet_queue()
        ,m_queue_mutex()
        ,m_queue_not_empty()
    {

    }

    virtual ~Impl(){}
};

Fake_device::Fake_device()
    :m_p_impl(new Impl)
{
}

Fake_device::~Fake_device()
{
    delete m_p_impl;
    m_p_impl = 0;
}

int64_t Fake_device::write(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms)
{
    int64_t bytes_written = 0;
    // std::unique_lock<std::timed_mutex> lock(m_p_impl->m_queue_mutex);
    if(m_p_impl->m_queue_mutex.try_lock_for(std::chrono::milliseconds(timeout_ms)))
    {
        m_p_impl->m_packet_queue.emplace(&out_bytes[0],&out_bytes[bytes_size]);
        m_p_impl->m_queue_not_empty.notify();
        m_p_impl->m_queue_mutex.unlock();
        bytes_written = static_cast<int64_t>(bytes_size);
    }
    else
    {
        bytes_written = static_cast<int64_t>(IDLP::IDLP_ERROR_TIMEOUT);
    }
    return bytes_written;
}

int64_t Fake_device::read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)
{
    int64_t bytes_read = 0;
    if(in_bytes == 0)
    {
        bytes_read = static_cast<int64_t>(IDLP::IDLP_ERROR_NULL);
    }
    else if(m_p_impl->m_queue_mutex.try_lock_for(std::chrono::milliseconds(timeout_ms)))
    {
        bool packet_queue_empty = m_p_impl->m_packet_queue.empty(); 
        if(packet_queue_empty)
        {
            packet_queue_empty = !m_p_impl->m_queue_not_empty.wait_for(timeout_ms);
        }
        
        if(!packet_queue_empty)
        {
            std::vector<uint8_t> packet = m_p_impl->m_packet_queue.front();
            bytes_read = packet.size();
            if(bytes_read > max_bytes_size)
            {
                bytes_read = max_bytes_size;
            }
            (void)std::memcpy(in_bytes, packet.data(), bytes_read);
            m_p_impl->m_packet_queue.pop();
        }
        else
        {
            bytes_read = static_cast<int64_t>(IDLP::IDLP_ERROR_TIMEOUT);
        }
        m_p_impl->m_queue_mutex.unlock();
    }
    else
    {
        bytes_read = static_cast<int64_t>(IDLP::IDLP_ERROR_TIMEOUT);
    }
    return bytes_read;
}

}
