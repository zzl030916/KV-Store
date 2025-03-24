#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "Timestamp.h"
#include "Timer.h"

#include <unistd.h>
#include <memory>
#include <vector>
#include <mutex>

namespace net 
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop 
{
    public:
        using Functor = std::function<void()>;

        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

        Timestamp pollReturnTime() const { return pollReturnTime_; }

        void runInLoop(const Functor& cb);
        void queueInLoop(const Functor& cb);

        Timer* runAt(const Timestamp& time, const TimerCallback& cb);
        Timer* runAfter(double delay, const TimerCallback& cb);
        Timer* runEvery(double interval, const TimerCallback& cb);

        void wakeup();
        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);

        void assertInLoopThread() 
        {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const { return threadId_ == gettid(); }

    private:
        void abortNotInLoopThread();
        void handleRead();
        void doPendingFunctors();

        using ChannelList = std::vector<Channel*>;

        bool looping_;
        bool quit_;
        bool callingPendingFunctors_;
        const pid_t threadId_;

        Timestamp pollReturnTime_;
        std::unique_ptr<Poller> poller_;
        std::unique_ptr<TimerQueue> timerQueue_;
        int wakeupFd_;
        std::unique_ptr<Channel> wakeupChannel_;
        ChannelList activeChannels_;
        std::mutex mutex_;
        std::vector<Functor> pendingFunctors_;
};

inline EventLoop* CheckNotNull(EventLoop* loop) 
{
    if (loop == NULL) {
        abort();
    }
    return loop;
}

}

#endif //NET_EVENTLOOP_H