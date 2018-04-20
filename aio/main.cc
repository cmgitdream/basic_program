
#include "thread.h"
#include "thread_pool.h"
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using std::string;
using namespace std;

const char *domains[] = {
"www.baidu.com",
"www.sina.com.cn",
"www.github.com",
"www.qq.com",
"www.taobao.com",
"www.toutiao.com",
"www.cctv.com",
"www.ifeng.com",
"www.linux.org",
"cn.bing.com",
"www.360.cn",
"www.alexa.com",
"www.alexa.cn",
"www.tmall.com",
"www.jd.com",
"www.weibo.com",
"www.alipay.com",
"www.csdn.net",
"www.hao123.com",
"www.so.com",
"www.sohu.com",
"www.163.com",
"www.tianya.cn",
"www.gmw.cn",
"www.soso.com",
"www.youth.cn",
"www.zhihu.com",
"www.xinhuanet.com"
};

int get_domain_ip(char *name);

int resolve_domain_from_file(char *file)
{
  int fd = 0;
  fd = open(file, O_RDWR, 0644);
  if (fd < 0) {
    cout << __func__ << ": open file " << file << " failed" << endl; 
    return -1;
  }
  return 0;
}

int task_callback_func(task_data_t arg)
{
  return get_domain_ip((char *)arg); 
}

int get_domain_ip(char *name)
{
  string domain = name;
  string cmd = "nslookup " + domain;
  return system(cmd.c_str());
}

int main(int argc, char *argv[])
{
  int i = 0;
  char *url;
  int count = sizeof(domains)/sizeof(domains[0]);
  cout << "count = " << count << endl;
  WorkQueue<Task> wq;
  ThreadPool<Task>  tp(8);
  tp.set_workqueue(&wq);
  tp.create_threads();
  for (; i < count; i++) {
    wq.wq_enqueue(new Task((task_data_t)domains[i], task_callback_func));
  }
  return 0;
  //return get_domain_ip(argv[1]);
}
