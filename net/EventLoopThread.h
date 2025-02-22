#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>

namespace net
{

class EventLoop;

class EventLoopThread
{
    public:
        EventLoopThread();
        ~EventLoopThread();
        EventLoop* startLoop();

    private:
        void threadFunc();

        EventLoop* loop_;
        bool exiting_;
        std::thread thread_;
        std::mutex mutex_;
        std::condition_variable cond_;
};

}

#endif  // NET_EVENTLOOPTHREAD_H

