
#ifndef _THREAD_H_
#define _THREAD_H_
#include <pthread.h>
#include <iostream>
#include <string>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using std::string;

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

class Mutex {
public:
  pthread_mutexattr_t mattr;
  pthread_mutex_t mutex;
  Mutex() {
    pthread_mutexattr_init(&mattr);
    pthread_mutex_init(&mutex, &mattr);
  }
  ~Mutex() {
    pthread_mutexattr_destroy(&mattr);
    pthread_mutex_destroy(&mutex);
  }
 
  int lock() {
    return pthread_mutex_lock(&mutex);
  } 

  int unlock() {
    return pthread_mutex_unlock(&mutex);
  }
};

class Cond {
public:
  pthread_condattr_t cattr;
  pthread_cond_t cond;
  Mutex *cond_mtx;
  bool paused;
  Cond(Mutex *cmtx):cond_mtx(cmtx), paused(false) {
    pthread_condattr_init(&cattr);
    pthread_cond_init(&cond, &cattr);
  }
  ~Cond() {
    pthread_condattr_destroy(&cattr);
    pthread_cond_destroy(&cond);
  }
  void set_mutex(Mutex *mtx) {
    cond_mtx = mtx;
  }
  int wait() {
    pthread_cond_wait(&cond, &cond_mtx->mutex);
  }
  int signal() {
    pthread_cond_signal(&cond);
  }
  int broadcast() {
    pthread_cond_broadcast(&cond);
  }
};

#endif //_THREAD_H_
