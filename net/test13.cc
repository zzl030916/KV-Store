#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpClient.h"


#include <utility>

#include <stdio.h>
#include <unistd.h>

std::string message = "Hello\n";

void onConnection(const net::TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        printf("onConnection(): new connection [%s] from %s\n",
            conn->name().c_str(),
            conn->peerAddress().toHostPort().c_str());
        conn->send(message);
    }
    else
    {
        printf("onConnection(): connection [%s] is down\n",
            conn->name().c_str());
    }
}

void onMessage(const net::TcpConnectionPtr& conn,
               net::Buffer* buf,
               Timestamp receiveTime)
{
    printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
            buf->readableBytes(),
            conn->name().c_str(),
            receiveTime.toFormattedString().c_str());

    printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

int main()
{
    net::EventLoop loop;
    
    net::InetAddress serverAddr("127.0.0.1", 9981);
    
    net::TcpClient client(&loop, serverAddr);

    
    client.setConnectionCallback(onConnection);
    client.setMessageCallback(onMessage);
    client.enableRetry();
    client.connect();
    loop.loop();
}

