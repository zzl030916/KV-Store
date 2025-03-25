#include "EventLoopThreadPool.h"

#include "EventLoop.h"
#include "EventLoopThread.h"

#include <assert.h>

using namespace net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop): 
    baseLoop_(baseLoop),
    started_(false),
    numThreads_(0),
    next_(0)
{
    
}

EventLoopThreadPool::~EventLoopThreadPool()
{
    
}

void EventLoopThreadPool::start()
{
    assert(!started_);
    baseLoop_->assertInLoopThread();

    started_ = true;

    // printf("EventLoopThreadPool::numThread::%d\n", numThreads_);
    for (int i = 0; i < numThreads_; ++i)
    {
        threads_.push_back(std::make_unique<EventLoopThread>());
        loops_.push_back(threads_[i]->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    baseLoop_->assertInLoopThread();
    EventLoop* loop = baseLoop_;

    // printf("EventLoopThreadPool::PoolSize::%d\n", (int)loops_.size());
    if (!loops_.empty())
    {
        // round-robin
        loop = loops_[next_];
        ++next_;
        if (static_cast<size_t>(next_) >= loops_.size())
        {
            next_ = 0;
        }
    }
    return loop;
}

