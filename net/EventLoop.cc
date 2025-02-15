#include "EventLoop.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <poll.h>


using namespace net;

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() : 
    looping_(false), 
    threadId_(gettid()) 
{
    printf("creat new EventLoop %p threadId %d\n", this, threadId_);
    if (t_loopInThisThread)
    {
        printf("another EventLoop %p exisit in this thread %d", t_loopInThisThread, threadId_);
        abort();
    }
    else
    {
        t_loopInThisThread = this;
    }

    
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = NULL;
    printf("threadId %d stop\n", threadId_);
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    printf("threadId %d is looping\n", threadId_);
    ::poll(NULL, 0, 5 * 1000);

    looping_ = false;
    printf("threadId %d exit loop\n", threadId_);
    
}

void EventLoop::abortNotInLoopThread()
{
    printf("EventLoop was created in threadId %d, current in threadId %d\n", threadId_, gettid());
    abort();
}
