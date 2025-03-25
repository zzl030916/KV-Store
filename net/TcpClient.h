#ifndef NET_TCPCLIENT_H
#define NET_TCPCLIENT_H

#include "TcpConnection.h"

#include <mutex>

namespace net
{

class Connector;
using ConnectorPtr = std::shared_ptr<Connector>;

class TcpClient
{
    public:
        TcpClient(EventLoop* loop, const InetAddress& serverAddr);
        ~TcpClient();  // force out-line dtor, for scoped_ptr members.

        void connect();
        void disconnect();
        void stop();

        TcpConnectionPtr connection() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return connection_;
        }

        bool retry() const;
        void enableRetry() { retry_ = true; }

        /// Set connection callback.
        /// Not thread safe.
        void setConnectionCallback(const ConnectionCallback& cb)
        { connectionCallback_ = cb; }

        /// Set message callback.
        /// Not thread safe.
        void setMessageCallback(const MessageCallback& cb)
        { messageCallback_ = cb; }

        /// Set write complete callback.
        /// Not thread safe.
        //   void setWriteCompleteCallback(const WriteCompleteCallback& cb)
        //   { writeCompleteCallback_ = cb; }

    private:
        /// Not thread safe, but in loop
        void newConnection(int sockfd);
        /// Not thread safe, but in loop
        void removeConnection(const TcpConnectionPtr& conn);

        EventLoop* loop_;
        ConnectorPtr connector_; // avoid revealing Connector
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        //   WriteCompleteCallback writeCompleteCallback_;
        bool retry_;   // atmoic
        bool connect_; // atomic
        // always in loop thread
        int nextConnId_;
        mutable std::mutex mutex_;
        TcpConnectionPtr connection_; // @BuardedBy mutex_
};

}

#endif  // NET_TCPCLIENT_H
