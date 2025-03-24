#ifndef NET_TCPCONNECTION_H
#define NET_TCPCONNECTION_H

#include "Buffer.h"
#include "InetAddress.h"
#include "Timestamp.h"
#include <functional>
#include <memory>


namespace net
{

class Channel;
class EventLoop;
class Socket;

class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void (const TcpConnectionPtr&)>;
using MessageCallback = std::function<void (const TcpConnectionPtr&, Buffer* buf, Timestamp)>;
using CloseCallback = std::function<void (const TcpConnectionPtr&)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
    public:
        TcpConnection(EventLoop* loop,
                    const std::string& name,
                    int sockfd,
                    const InetAddress& localAddr,
                    const InetAddress& peerAddr);
        ~TcpConnection();
    
        EventLoop* getLoop() const { return loop_; }
        const std::string& name() const { return name_; }
        const InetAddress& localAddress() { return localAddr_; }
        const InetAddress& peerAddress() { return peerAddr_; }
        bool connected() const { return state_ == kConnected; }
    
        void send(const std::string& message);

        void shutdown();

        void setConnectionCallback(const ConnectionCallback& cb)
        { connectionCallback_ = cb; }
    
        void setMessageCallback(const MessageCallback& cb)
        { messageCallback_ = cb; }
    
        
        void setCloseCallback(const CloseCallback& cb)
        { closeCallback_ = cb; }
    
        
        void connectEstablished();   
        
        void connectDestroyed(); 
    
   private:
        enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected, };
    
        void setState(StateE s) { state_ = s; }
        void handleRead(Timestamp receiveTime);
        void handleWrite();
        void handleClose();
        void handleError();
        void sendInLoop(const std::string& message);
        void shutdownInLoop();
    
        EventLoop* loop_;
        std::string name_;
        StateE state_; 
        std::unique_ptr<Socket> socket_;
        std::unique_ptr<Channel> channel_;
        InetAddress localAddr_;
        InetAddress peerAddr_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        CloseCallback closeCallback_;
        Buffer inputBuffer_;
        Buffer outputBuffer_;
};

}

#endif  // NET_TCPCONNECTION_H