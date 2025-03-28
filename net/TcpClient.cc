// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "TcpClient.h"

#include "Connector.h"
#include "EventLoop.h"


#include <stdio.h>  // snprintf
#include <cstring>

using namespace net;

namespace net
{
namespace detail
{

void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
{
    loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

}
}

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr): 
    loop_(CheckNotNull(loop)),
    connector_(new Connector(loop, serverAddr)),
    retry_(false),
    connect_(true),
    nextConnId_(1)
{
    connector_->setNewConnectionCallback(
        std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
    // FIXME setConnectFailedCallback
    // LOG_INFO << "TcpClient::TcpClient[" << this
    //         << "] - connector " << get_pointer(connector_);
}

TcpClient::~TcpClient()
{
    // LOG_INFO << "TcpClient::~TcpClient[" << this
    //          << "] - connector " << get_pointer(connector_);
    TcpConnectionPtr conn;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        conn = connection_;
    }
    if (conn)
    {
        // FIXME: not 100% safe, if we are in different thread
        CloseCallback cb = std::bind(&detail::removeConnection, loop_, std::placeholders::_1);
        loop_->runInLoop(
            std::bind(&TcpConnection::setCloseCallback, conn, cb));
    }
    else
    {
        connector_->stop();
    }
}

void TcpClient::connect()
{
  // FIXME: check state
  // LOG_INFO << "TcpClient::connect[" << this << "] - connecting to "
  //          << connector_->serverAddress().toHostPort();
  connect_ = true;
  connector_->start();
}

void TcpClient::disconnect()
{
  connect_ = false;

  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (connection_)
    {
      connection_->shutdown();
    }
  }
}

void TcpClient::stop()
{
  connect_ = false;
  connector_->stop();
}

void TcpClient::newConnection(int sockfd)
{
    loop_->assertInLoopThread();

    struct sockaddr_in peeraddr;
    bzero(&peeraddr, sizeof peeraddr);
    socklen_t addrlen = sizeof peeraddr;
    InetAddress peerAddr(getpeername(sockfd, InetAddress::sockaddr_cast(&peeraddr), &addrlen));
    char buf[32];
    snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toHostPort().c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = buf;

    struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof localaddr);
    InetAddress localAddr(getsockname(sockfd, InetAddress::sockaddr_cast(&peeraddr), &addrlen));

    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(loop_,
                                            connName,
                                            sockfd,
                                            localAddr,
                                            peerAddr));

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    // conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(
        std::bind(&TcpClient::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    {
        std::lock_guard<std::mutex> lock(mutex_);
        connection_ = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
  loop_->assertInLoopThread();
  assert(loop_ == conn->getLoop());

  {
    std::lock_guard<std::mutex> lock(mutex_);
    assert(connection_ == conn);
    connection_.reset();
  }

  loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
  if (retry_ && connect_)
  {
    // LOG_INFO << "TcpClient::connect[" << this << "] - Reconnecting to "
    //          << connector_->serverAddress().toHostPort();
    connector_->restart();
  }
}

