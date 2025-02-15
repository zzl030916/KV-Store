#include "EventLoop.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>

void threadFunc()
{
  printf("threadFunc(): pid = %d, tid = %d\n",
         getpid(), gettid());

  net::EventLoop loop;
  loop.loop();
  
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), gettid());

  net::EventLoop loop;

  std::thread thread(threadFunc);
  thread.join();

  loop.loop();
  pthread_exit(NULL);
}
