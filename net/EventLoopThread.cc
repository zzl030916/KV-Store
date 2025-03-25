#include "EventLoopThread.h"

#include "EventLoop.h"

using namespace net;

EventLoopThread::EventLoopThread():
    loop_(NULL),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this))
{
    // printf("EventLoopThread::tid::%d\n", gettid());
}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    loop_->quit();
    thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [&]{ return loop_ != NULL; });
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_all();
    }

    loop.loop();
}