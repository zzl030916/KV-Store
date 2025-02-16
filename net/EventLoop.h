#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include <unistd.h>
#include <memory>
#include <vector>

namespace net 
{

class Channel;
class Poller;

class EventLoop 
{
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

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

        std::unique_ptr<Poller> poller_;
        ChannelList activeChannels_;
};

}

#endif //NET_EVENTLOOP_H