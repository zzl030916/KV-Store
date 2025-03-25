#include "Socket.h"

#include "InetAddress.h"


#include <unistd.h>
#include <netinet/tcp.h>
#include <strings.h>  // bzero

using namespace net;

int Socket::createNonblockingOrDie()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
        IPPROTO_TCP);
    if (sockfd < 0)
    {
        abort();
    }
    return sockfd;
}

Socket::~Socket()
{
    if (::close(sockfd_) < 0)
    {
        abort();
    }
}

void Socket::bindAddress(const InetAddress& addr)
{
    if(::bind(sockfd_, InetAddress::sockaddr_cast(&addr.getSockAddrInet()), sizeof addr.getSockAddrInet()) < 0)
    {
        abort();
    }
}

void Socket::listen()
{
    if (::listen(sockfd_, SOMAXCONN) < 0)
    {
        abort();
    }
}

int Socket::accept(InetAddress* peeraddr)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    bzero(&addr, sizeof addr);
    int connfd = ::accept4(sockfd_, InetAddress::sockaddr_cast(&addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        abort();
    }
    peeraddr->setSockAddrInet(addr);
    return connfd;
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0)
    {
        abort();
    }
}

void Socket::shutdownWrite()
{
    if (::shutdown(sockfd_, SHUT_WR) < 0)
    {
        // LOG_SYSERR << "sockets::shutdownWrite";
    }
}

void Socket::setTcpNoDelay(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);
}

