/// @file IDLP/IDLP_Semaphore.cpp

#include <IDLP/IDLP_Semaphore.h>
#include <mutex>
#include <condition_variable>

namespace IDLP
{

struct IDLP_Semaphore::Impl
{
public:
    std::mutex m_mutex;
    std::condition_variable m_condition_variable;
    uint64_t m_extra_notifications = 0; // Initialized as locked.

    Impl(){}
    virtual ~Impl(){}
};

IDLP_Semaphore::IDLP_Semaphore()
    :m_p_impl(new Impl)
{
}

IDLP_Semaphore::~IDLP_Semaphore()
{
    delete m_p_impl;
    m_p_impl = 0;
}

bool IDLP_Semaphore::wait_for(size_t timeout_ms)
{
    bool result = false;
    std::unique_lock<std::mutex> lock(m_p_impl->m_mutex);
    if(m_p_impl->m_extra_notifications > 0) // Handle spurious wake-ups.
    {
        result = (m_p_impl->m_condition_variable.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::no_timeout);
        if(result)
        {
            --m_p_impl->m_extra_notifications;
        }
    }
    return result;
}

void IDLP_Semaphore::notify()
{
    std::lock_guard<std::mutex> lock(m_p_impl->m_mutex);
    ++m_p_impl->m_extra_notifications;
    m_p_impl->m_condition_variable.notify_one();
}

}
