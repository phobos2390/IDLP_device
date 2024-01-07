/// @file IDLP/IDLP_Semaphore.h

#ifndef IDLP_IDLP_SEMAPHORE_H
#define IDLP_IDLP_SEMAPHORE_H

#include <IDLP/IDLP_Errors.h>
#include <cstdlib>

namespace IDLP
{

class IDLP_Semaphore
{
public:
    /// Creates class value
    IDLP_Semaphore();

    /// Removes class value
    virtual ~IDLP_Semaphore();

    /// @brief Waits for the designated amount of time for the semaphore signal
    /// @param[in] timeout_ms - max amount of time to wait in milliseconds for the signal
    /// @return true if received signal
    bool wait_for(size_t timeout_ms);
    
    /// @brief notifies the other thread
    void notify();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* IDLP_IDLP_SEMAPHORE_H */
