
#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include "thread.h"

#define gettid() syscall(SYS_gettid)

using namespace std;

Mutex mtx;		// mutex for list
Mutex mtx_cond;		// mutex for cond
Cond cond_p(&mtx); // cond for producer
Cond cond_c(&mtx); // cond for consumer
int data_max = 5;
list<int> data;

void print_list(list<int>& data) {
  list<int>::iterator itr = data.begin();
  cout << "list: size = " << data.size()
	<< ", max size = " << data_max
	<< " --> [ ";
  for (; itr != data.end(); itr++)
    cout << *itr << ", ";
  cout << " ]" <<endl;
}

class Producer : public Thread {
public:
  Producer() {}
  ~Producer() {}
  void * real_entry() {
    while(1) {
      struct timeval tv;
      int n;
      mtx.lock();
      if (data.size() >= data_max)
        cout << "[Producer] "<< getpid() << "." << pthread_self() << ": wait ..., data.size = " << data.size() << endl;
      while (data.size() >= data_max) {
        //cout << getpid() << "." << pthread_self() << ": wait producer " << endl;
	cond_p.wait();
      }
      gettimeofday(&tv, NULL); 
      srandom(tv.tv_sec);
      n = random();
      data.push_back(n); 
      cout << "[Producer] " << getpid() << "." << pthread_self() << ": generate " << n << endl;
      print_list(data);
      if (data.size() > 0) {
        cout << "[Producer] " << getpid() << "." << pthread_self() << ": signal consumer " << endl;
        cond_c.signal();
      }
      mtx.unlock();
      sleep(2);
    }
  }
};

class Consumer : public Thread {
public:
  Consumer() {}
  ~Consumer() {}
  void * real_entry() {
    while(1) {
      int x;
      int count;
      mtx.lock();
      count = data.size();
      if (data.size() == 0)
        cout << "[Consumer] " << getpid() << "." << pthread_self() << ": wait ..., data.size = " << data.size() << endl;
      while (data.size() == 0) {
        //cout << getpid() << "." << pthread_self() << ": wait consumer " << endl;
	cond_c.wait();
      }
      x = data.front(); 
      data.pop_front();
      cout << "[Consumer] " << getpid() << "." << pthread_self() << ": consume " << x << ", after size = " << data.size() << endl;
      if (data.size() == 0) {
        cout << "[Consumer] " << getpid() << "." << pthread_self() << ": broadcast producer " << endl;
	cond_p.signal();
      }
      mtx.unlock();
      sleep(1);
    }
  }
};

int main(int argc, char *argv[])
{
  Producer p1, p2, p3;
  Consumer c1;

  p1.create("producer1");
  p1.create("producer2");
  p1.create("producer3");
  c1.create("consumer1");
  c1.create("consumer2");

  p1.join();
  p2.join();
  p3.join();
  c1.join();
  return 0;
}
