#include "EventLoop.h"

#include "Poller.h"
#include "Channel.h"
#include "TimerQueue.h"

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
    poller_(new Poller(this)),
    timerQueue_(new TimerQueue(this))
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
    looping_ = false;
}

void EventLoop::quit() 
{
    quit_ = true;
}

Timer* EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
  return timerQueue_->addTimer(cb, time, 0.0);
}

Timer* EventLoop::runAfter(double delay, const TimerCallback& cb)
{
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, cb);
}

Timer* EventLoop::runEvery(double interval, const TimerCallback& cb)
{
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue_->addTimer(cb, time, interval);
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
