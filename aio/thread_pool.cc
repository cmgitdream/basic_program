
#include "thread_pool.h"

//template<typename T> WorkQueue<T> *ThreadPool<T>::tp_queue = NULL;

template<typename T>
void ThreadPool<T>::set_workqueue(WorkQueue<T> *tq) { 
  tp_queue = tq;
}
