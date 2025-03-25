#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <stdio.h>

void onConnection(const net::TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        printf("onConnection(): tid=%d new connection [%s] from %s\n",
            gettid(),
            conn->name().c_str(),
            conn->peerAddress().toHostPort().c_str());
    }
    else
    {
        printf("onConnection(): tid=%d connection [%s] is down\n",
            gettid(),
            conn->name().c_str());
    }
}

void onMessage(const net::TcpConnectionPtr& conn, net::Buffer* buf, Timestamp receiveTime)
{
    printf("onMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
            gettid(),
            buf->readableBytes(),
            conn->name().c_str(),
            receiveTime.toFormattedString().c_str());

    conn->send(buf->retrieveAsString());
}

int main(int argc, char* argv[])
{
    printf("main(): pid = %d\n", getpid());

    net::InetAddress listenAddr(9981);

    
    net::EventLoop loop;
    net::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);

    
    if (argc > 1) {
        server.setThreadNum(atoi(argv[1]));
    }
    server.start();

    loop.loop();
}
