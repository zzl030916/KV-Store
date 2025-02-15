#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <unistd.h>

namespace net 
{

class EventLoop 
{
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void assertInLoopThread() 
        {
            if (!isInLoopThread()) 
            {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const { return threadId_ == gettid(); }

    private:
        void abortNotInLoopThread();

        bool looping_;
        const pid_t threadId_;
};

}

#endif