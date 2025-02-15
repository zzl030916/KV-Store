#include "EventLoop.h"
#include <thread>

net::EventLoop* g_loop;

void threadFunc()
{
  g_loop->loop();
}

int main()
{
  net::EventLoop loop;
  g_loop = &loop;
  std::thread t(threadFunc);
  t.join();
}
