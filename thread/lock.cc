
#include <pthread.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


std::mutex mtx;
bool shutdown;
int n;
pthread_t tid;
pthread_attr_t attr;

void sig_usr(int signum)
{
  std::cout << "signal = " << strsignal(signum) << std::endl;
}

void *thread_entry(void *arg)
{
  while(!shutdown) {
    std::unique_lock<std::mutex> uniq(mtx);
    std::cout << "n = " << n++ << std::endl;
    uniq.unlock();
    //std::this_thread::sleep_for(std::chrono::milliseconds(120));
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  pid_t child_pid;
  int dev_null_fd;
  signal(SIGUSR1, sig_usr);

/*
  switch(child_pid = fork()) {
  case -1:
    std::cout << "fork failed" << std::endl;
    break;
  case 0: // I am child
    if (setsid() == -1) {
      std::cout << " setsid failed, errno = " << errno << std::endl;
    }
  default:
    std::cout << " I am parent, should exit" << std::endl;  
    exit(0);
    break;
  }
*/

  int ret;
  ret = pthread_create(&tid, NULL, thread_entry, NULL); 
  if (ret < 0) 
  {
    std::cout << " phtread create failed ret = " << ret << std::endl;
  }
  pthread_kill(tid, SIGUSR1);
  pthread_join(tid, NULL);
  std::cout << " main process end" << std::endl;
}

