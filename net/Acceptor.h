#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include "Channel.h"
#include "Socket.h"

namespace net
{

class EventLoop;
class InetAddress;

///
/// Acceptor of incoming TCP connections.
///
class Acceptor
{
    public:
        using NewConnectionCallback = std::function<void (int sockfd, const InetAddress&)>;

        Acceptor(EventLoop* loop, const InetAddress& listenAddr);

        void setNewConnectionCallback(const NewConnectionCallback& cb)
        { newConnectionCallback_ = cb; }

        bool listenning() const { return listenning_; }
        void listen();

    private:
        void handleRead();

        EventLoop* loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        NewConnectionCallback newConnectionCallback_;
        bool listenning_;
};

}

#endif  // NET_ACCEPTOR_H
