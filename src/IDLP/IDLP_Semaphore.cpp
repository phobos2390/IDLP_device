/// @file IDLP/IDLP_Semaphore.cpp

#include <IDLP/IDLP_Semaphore.h>
#include <mutex>
#include <condition_variable>

namespace IDLP
{

struct IDLP_Semaphore::Impl
{
public:
    std::mutex mutex_;
    std::condition_variable condition_;
    uint64_t count_ = 0; // Initialized as locked.

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
    std::unique_lock<std::mutex> lock(m_p_impl->mutex_);
    if(m_p_impl->count_ > 0) // Handle spurious wake-ups.
    {
        result = (m_p_impl->condition_.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::no_timeout);
        if(result)
        {
            --m_p_impl->count_;
        }
    }
    return result;
}

void IDLP_Semaphore::notify()
{
    std::lock_guard<std::mutex> lock(m_p_impl->mutex_);
    ++m_p_impl->count_;
    m_p_impl->condition_.notify_one();
}

}
