#include "Connector.h"
#include "EventLoop.h"

#include <stdio.h>

net::EventLoop* g_loop;

void connectCallback(int sockfd)
{
  printf("connected.\n");
  g_loop->quit();
}

int main(int argc, char* argv[])
{
    net::EventLoop loop;
    g_loop = &loop;
    net::InetAddress addr("127.0.0.1", 9981);
    net::ConnectorPtr connector(new net::Connector(&loop, addr));
    connector->setNewConnectionCallback(connectCallback);
    connector->start();

    loop.loop();
}
