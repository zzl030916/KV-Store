#ifndef NET_POLLER_H
#define NET_POLLER_H

#include <map>
#include <vector>
#include <stdint.h>

#include "EventLoop.h"
#include "Timestamp.h"

struct pollfd;

namespace net
{

class Channel;


class Poller
{
    public:
        using ChannelList = std::vector<Channel*>;

        Poller(EventLoop* loop);
        ~Poller();

        Timestamp poll(int timeoutMs, ChannelList* activeChannels);

        void updateChannel(Channel* channel);

        void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

    private:
        void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

        using PollFdList = std::vector<struct pollfd>;
        using ChannelMap = std::map<int, Channel*>;

        EventLoop* ownerLoop_;
        PollFdList pollfds_;
        ChannelMap channels_;
};

}

#endif //NET_POLLER_H