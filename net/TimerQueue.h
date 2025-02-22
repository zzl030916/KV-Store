#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

#include "Channel.h"
#include "Timestamp.h"

#include <set>
#include <vector>
#include <functional>

namespace net
{

class EventLoop;
class Timer;

class TimerQueue
{
    public:
        using TimerCallback = std::function<void()>;

        TimerQueue(EventLoop* loop);
        ~TimerQueue();

        Timer* addTimer(const TimerCallback& cb, Timestamp when, double interval);

    private:
        using Entry = std::pair<Timestamp, Timer*>;
        using TimerList = std::set<Entry>;

        void addTimerInLoop(Timer* timer);

        void handleRead();

        std::vector<Entry> getExpired(Timestamp now);
        void reset(const std::vector<Entry>& expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_;
        Channel timerfdChannel_;
        // Timer list sorted by expiration
        TimerList timers_;
};


}
#endif  // NET_TIMERQUEUE_H
