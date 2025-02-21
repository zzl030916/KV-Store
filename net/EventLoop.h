#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "Timestamp.h"
#include "Timer.h"

#include <unistd.h>
#include <memory>
#include <vector>

namespace net 
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop 
{
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

        Timestamp pollReturnTime() const { return pollReturnTime_; }

        Timer* runAt(const Timestamp& time, const TimerCallback& cb);
        Timer* runAfter(double delay, const TimerCallback& cb);
        Timer* runEvery(double interval, const TimerCallback& cb);


        void updateChannel(Channel* channel);

        void assertInLoopThread() 
        {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const { return threadId_ == gettid(); }

    private:
        void abortNotInLoopThread();

        using ChannelList = std::vector<Channel*>;

        bool looping_;
        bool quit_;
        const pid_t threadId_;

        Timestamp pollReturnTime_;
        std::unique_ptr<Poller> poller_;
        std::unique_ptr<TimerQueue> timerQueue_;
        ChannelList activeChannels_;
};

}

#endif //NET_EVENTLOOP_H