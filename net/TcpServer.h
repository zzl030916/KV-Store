#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "TcpConnection.h"

#include <map>

namespace net
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer
{
 public:

    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer();  // force out-line dtor, for scoped_ptr members.

    /// Starts the server if it's not listenning.
    ///
    /// It's harmless to call it multiple times.
    /// Thread safe.
    void start();

    /// Set connection callback.
    /// Not thread safe.
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    /// Set message callback.
    /// Not thread safe.
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }


    void setThreadNum(int numThreads);

    private:
        /// Not thread safe, but in loop
        void newConnection(int sockfd, const InetAddress& peerAddr);
        void removeConnection(const TcpConnectionPtr& conn);
        void removeConnectionInLoop(const TcpConnectionPtr& conn);

        typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

        EventLoop* loop_;  // the acceptor loop
        const std::string name_;
        std::unique_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
        std::unique_ptr<EventLoopThreadPool> threadPool_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        bool started_;
        int nextConnId_;  // always in loop thread
        ConnectionMap connections_;
};

}

#endif  // NET_TCPSERVER_H
