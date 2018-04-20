
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <attr/xattr.h>

#define GC_DIR "dir"
#define GC_PERIOD 30
// in case two gc processes or threads operate on this series files
#define GC_LOCK "gc_lock"
#define GC_LABEL_NAME "user.pacs.gc.finish"
#define GC_LABEL_VAL "1"

sigset_t set;
pthread_cond_t cond;
pthread_mutex_t mtx;
int shutdown = 0;

int check_gc_label(char *path)
{
  int len = 0;
  char val[8];
  memset(val, 0, 8);
  errno = 0;
  len = getxattr(path, GC_LABEL_NAME, val, 8); 
  if (len < 0 ) {
    if (errno == ENOATTR) {
      printf("%s: file %s has no xattr %s, will be gc\n", __func__, path, GC_LABEL_NAME);
      return -1;
    }
  }
  printf("%s: file %s is written completely\n", __func__, path);
  return 0;
}

int set_gc_label(char *path)
{
  int len = 0; 
  errno = 0;
  len = setxattr(path, GC_LABEL_NAME, GC_LABEL_VAL, strlen(GC_LABEL_VAL)+1,
	XATTR_CREATE);
  if (len < 0) {
    printf("%s: file %s, setxattr failed %d(%s)\n", __func__, path, errno, strerror(errno));
    return -1;
  }
  return 0;
}

int readdir_wrapper(char *path)
{
  DIR *dirp = opendir(path);
  if (dirp == NULL) {
    printf("%s: opedir %s failed\n", __func__, path);
    return -1;
  }
  int len = strlen(path) + 256;
  char *file = (char *)malloc(len);
  if (file == NULL) {
    printf("malloc error\n");
    return -1;
  }
  struct dirent *ent = NULL;
  do {
    errno = 0;
    
    if((ent = readdir(dirp)) != NULL) {
      printf("%s\n", ent->d_name);
      if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0
	&& strcmp(ent->d_name, GC_LOCK) != 0) {
      //if (ent->d_type ==  DT_REG) {
        memset(file, '\0', len);
        snprintf(file, len, "%s/%s", path, ent->d_name);
	check_gc_label(file);
      //}
      }
    }
  } while(ent != NULL);
  closedir(dirp);
  if (errno != 0) {
    perror("readir_wrapper: readdir error\n");
    free(file);
    return -1;
  }
  free(file);
  return 0;
}

void *gc_process(void *arg)
{
  char *path = (char *)arg;
  int len = strlen(path) + strlen(GC_LOCK) + 2;
  char *lock = (char *)malloc(len);
  if (lock == NULL) {
    printf("malloc error\n");
    return NULL;
  }
  memset(lock, '\0', len);
  snprintf(lock, len, "%s/%s", path, GC_LOCK);
  errno = 0;
  if (access(lock, F_OK) == 0) {
    printf("gc is blocked by other process\n");
    free(lock);
    return NULL;
  }
  if (open(lock, O_CREAT|O_WRONLY|O_TRUNC, 0644) < 0) {
      printf("create file %s failed\n", lock);
      free(lock);
      return NULL;
  } 
  int stop = 0;
  while (1) {
    pthread_mutex_lock(&mtx);
    stop = shutdown;
    pthread_mutex_unlock(&mtx);
    if (stop)
      break;
    // do disk fsck
    if(readdir_wrapper(path) < 0) {
      free(lock);
      return NULL;
    }
    sleep(GC_PERIOD);
  }

  printf("recv signal to shutdown\n");
  unlink(lock); 
  free(lock);
  return NULL;
}

void *sig_thread(void *arg)
{
  sigset_t *set = (sigset_t *)arg;
  int s, sig;
  for (;;) {
    s = sigwait(set, &sig);
    if (s != 0) {
      printf("sigwait failed\n");
      exit(1);
    }
    printf("got signal %d\n", sig);
    pthread_mutex_lock(&mtx);  
    shutdown = 1;
    pthread_mutex_unlock(&mtx);  
  }
}

void usage()
{
  printf("./gc <dir>");
}



int main(int argc, char *argv[])
{
  if (argc < 2) {
    usage();
    exit(1);
  }
  pthread_t tid1, tid2;
  int ret1, s;
  
  pthread_mutex_init(&mtx, NULL);
  pthread_cond_init(&cond, NULL);

  sigaddset(&set, SIGTERM);
  sigaddset(&set, SIGHUP);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);

  s = pthread_sigmask(SIG_BLOCK, &set, NULL);
  if (s != 0) {
    printf("pthread_sigmask error\n");
    return 1; 
  }

  pthread_create(&tid1, NULL, gc_process, argv[1]);
  pthread_create(&tid2, NULL, sig_thread, &set);
  pthread_join(tid1, NULL);
  pthread_mutex_destroy(&mtx);
  pthread_cond_destroy(&cond);
  return 1;
}
