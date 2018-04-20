
#include <pthread.h>
#include <iostream>
#include <map>
#include <queue>
#include <list>
#include <string>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using std::map;
using std::string;
using std::list;

#define PTHREAD_NAME_MAX 255

class Thread {
public:
  pthread_t tid;
  pthread_attr_t attr;
  int name_max_len;
  Thread(int max_len = PTHREAD_NAME_MAX) : name_max_len(max_len) {
    pthread_attr_init(&attr);
  }
  ~Thread() { pthread_attr_destroy(&attr);}
protected:
  virtual void * real_entry() = 0;
private:
  static void *entry_wrapper(void *arg) {
    return ((Thread *)arg)->real_entry(); 
  }
public:
  int create(const char *thread_name) {
    int ret;
    
    ret = pthread_create(&tid, &attr, entry_wrapper, (void *)this);
    if (ret < 0) {
      std::cout << __func__ << ": pthread create failed, ret = " << ret << std::endl;
      return ret;
    }
    ret = pthread_setname_np(tid, thread_name);
    return ret;
  }
  int join() {
    int ret = 0;
    int *pret = &ret;
    return pthread_join(tid, (void **)&pret);
  }
  int detach() {
    return pthread_detach(tid);
  }
  int thread_name(string name) {
    char pname[name_max_len];
    memset(pname, '\0', name_max_len);
    pthread_getname_np(tid, pname, name_max_len);
    name = pname;
    
  }
};

typedef void (*aio_callback)(void *arg);
typedef void * cb_type;

struct AioCompletion {
  aio_callback aio_cb;
  cb_type aio_arg;
};

class SleepThread : public Thread {
public:
  AioCompletion *comp;
  SleepThread(AioCompletion *cmp):comp(cmp) {}
  void *real_entry(void *arg) {
    int *id = (int *)comp->aio_arg;
    sleep(2);
    std::cout << *id << "sleep 2 ... " << std::endl;  
    return NULL;
  }
};

// all aio is pended by me
class AioProcessor : public Thread {
public:
  pthread_mutex_t aio_mutex;
  pthread_cond_t aio_cond;
  list<AioCompletion *> pending_list;
  AioProcessor() {
    pthread_mutex_init(&aio_mutex, NULL);
    pthread_cond_init(&aio_cond, NULL);
  }
  
  void *real_entry(){
    while (1) {
      pthread_mutex_lock(&aio_mutex);
      while (pending_list.empty())
	pthread_cond_wait(&aio_cond, &aio_mutex);
      pthread_mutex_unlock(&aio_mutex);
      AioCompletion *comp = dequeue();
      //SleepThread *slp = new SleepThread(comp);
      if (comp->aio_cb) {
        //slp->create("sleep thread");
	comp->aio_cb(comp->aio_arg);
      }
      delete comp;
      //delete slp;
    }
    return NULL;
  }

  void enqueue(AioCompletion *comp) {
    pending_list.push_back(comp);
  }

  AioCompletion *dequeue() {
    AioCompletion *comp = pending_list.front();
    pending_list.pop_front();
    return comp;
  }
  
};

void sleep_echo(void *arg)
{
  int *id = (int *)arg;
  sleep(2);
  std::cout << *id << "sleep 2 ... " << std::endl;  
}


#define N 20

int main(int argc, char *argv[])
{
  AioProcessor aiopsor;
  AioCompletion *arr[N];
  int ids[N];  
  int i = 0;

  aiopsor.create("aio_processor");
  for (; i < N; i++) {
    arr[i] = new AioCompletion();
    arr[i]->aio_cb = sleep_echo;
    ids[i] = i;
    arr[i]->aio_arg = (void *)&ids[i];
    aiopsor.enqueue(arr[i]);
  }
  pthread_cond_signal(&aiopsor.aio_cond);
  aiopsor.join();
  std::cout << " end" << std::endl;
}


