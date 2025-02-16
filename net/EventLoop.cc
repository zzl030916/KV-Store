#include "EventLoop.h"

#include "Poller.h"
#include "Channel.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <poll.h>


using namespace net;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop() : 
    looping_(false), 
    quit_(false),
    threadId_(gettid()),
    poller_(new Poller(this)) 
{
    printf("creat new EventLoop %p threadId %d\n", this, threadId_);
    if (t_loopInThisThread) {
        printf("another EventLoop %p exisit in this thread %d", t_loopInThisThread, threadId_);
        abort();
    }
    else {
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
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);
        for (ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->headleEvent();
        }
    }

    printf("EventLoop %p stop looping\n", this);
    
}

void EventLoop::quit() 
{
    quit_ = true;
}

void EventLoop::updateChannel(Channel* channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
    printf("EventLoop was created in threadId %d, current in threadId %d\n", threadId_, gettid());
    abort();
}
