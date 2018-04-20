
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
#include <stdio.h>
#include <queue>
#include <list>
#include "thread.h"

using std::queue;
using std::list;
#define WQ_MAX_SIZE 1024

typedef void * task_data_t;
typedef int (*task_callback_t )(task_data_t arg);

class Task {
public:
  task_data_t tsk_data;
  task_callback_t tsk_cb;
  Task(task_data_t data, task_callback_t cb) :
    tsk_data(data), tsk_cb(cb)
  {}
  ~Task() {}
  int task_run() {
    return tsk_cb(tsk_data);
  }
};

// task queue
template<typename T>
class WorkQueue {
public:
  int wq_max; // queue max size
  queue<T *> wq_queue; // task event queue
  Mutex wq_mutex;
  WorkQueue(int max = WQ_MAX_SIZE):wq_max(max) {
  }
  ~WorkQueue() {
  }
  int wq_enqueue(T *item) {
    // wq_queue is full, reject insert item
    if (wq_queue.size() + 1 > wq_max) {
      return -1;
    }
    wq_mutex.lock(); 
    wq_queue.push(item);
    wq_mutex.unlock(); 
    return 0;
  }

  T *wq_dequeue() {
    wq_mutex.lock(); 
    T *first = wq_queue.front();
    wq_queue.pop();
    wq_mutex.unlock(); 
    return first;
  }

  size_t wq_size() {
    return wq_queue.size();
  }
};


template<typename T>
class ThreadPool {
public:
  //static WorkQueue<T> *tp_queue;
  WorkQueue<T> *tp_queue;
  int tp_max_threads;
  list<Thread *> tp_threads;
  
  ThreadPool(int nthreads)
    :tp_max_threads(nthreads)
  {
  }
  ~ThreadPool()
  {
    int threads = tp_threads.size();
    int i = 0;
    list<Thread *>::iterator itr = tp_threads.begin();
    for (; itr != tp_threads.end(); itr++) {
      // to do: wait thread to complete?
      delete *itr;
    } 
    tp_threads.clear();
  }
  T *pop_queue() {
    return tp_queue->wq_dequeue();
  }

  int get_queue_size() {
    return tp_queue->wq_size();
  }

  void set_workqueue(WorkQueue<T> *tq);

  class WorkThread;
  void worker_entry(WorkThread *wthrd) {
    while (1) {
      wt_cond->paused = (get_queue_size() == 0);
	if (!wt_cond->paused) {
	  T *task = pop_queue();
//ThreadPool::tp_queue->wq_dequeue();
	  // run task callback
	  task->task_run();
	  delete task;
	} else {
	  wt_cond->wait();
	}
    
  }

  class WorkThread : public Thread {
  public:
    Mutex *wt_mtx;
    Cond *wt_cond;
    bool running;
    ThreadPool *tpool;
    WorkThread(ThreadPool *pool) :
	Thread(),running(false), tpool(pool) {
      wt_mtx = new Mutex();
      wt_cond = new Cond(wt_mtx); 
    }
    ~WorkThread() {
      delete wt_mtx;
      delete wt_cond;
    }
    void *real_entry() {
	}
      }
    }
  };

  int create_threads(char *prefix = NULL) {
    int i = 0;
    int n = 0;
    char tname[32];
    Thread *thrd;
    for (i = 0; i < tp_max_threads; i++) {
      snprintf(tname, 32, "%s-%i", (prefix ? prefix : "tpthread"), i);
      tname[n] = '\0';
      thrd = new WorkThread(); 
      thrd->create(tname);     
    }
  }
  
};

#endif //_THREAD_POOL_H_
