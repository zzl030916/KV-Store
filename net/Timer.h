#ifndef NET_TIMER_H
#define NET_TIMER_H

#include "Timestamp.h"
#include <atomic>
#include <functional>

using TimerCallback = std::function<void()>;

namespace net
{

class Timer
{
    public:
        Timer(const TimerCallback& cb, Timestamp when, double interval) :
            callback_(cb),
            expiration_(when),
            interval_(interval),
            repeat_(interval > 0.0),
            sequence_(s_numCreated_.fetch_add(1, std::memory_order_relaxed))
    { 
    }

    void run() const
    {
        callback_();
    }

    Timestamp expiration() const  { return expiration_; }
    bool repeat() const { return repeat_; }
    int64_t sequence() const { return sequence_; }

    void restart(Timestamp now);

    private:
        const TimerCallback callback_;
        Timestamp expiration_;
        const double interval_;
        const bool repeat_;
        const int64_t sequence_;

        static std::atomic<int64_t> s_numCreated_;
};

}
#endif  // NET_TIMER_H
