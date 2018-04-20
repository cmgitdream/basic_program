
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
  int r = 0;
  char buf[BUFSIZ];
  char hello[] = "hello child";
  // fd[0]: read
  // fd[1]: write
  int fd[2];
  r = socketpair(AF_UNIX, SOCK_STREAM, 0, fd); 
  if (r < 0) {
    printf("error %d on socketpair ", errno);
    exit(1);
  }
  pid_t child_pid = 0;
  // all vairalbes before fork() are shared by both parent and child
  child_pid = fork();
  if (child_pid < 0) {
    printf("parent: child fork failed ,errno = %d\n", errno);
  }
  else if (child_pid > 0) {
    printf("fork child_pid: %lu\n", child_pid);
    close(fd[0]);
    if (write(fd[1], hello, strlen(hello)*sizeof(char)) < 0) {
      printf("parent write failed, errno = %d\n", errno);
      exit(0);
    }
    close(fd[1]);
    int status = -1;
    pid_t wpid = -1;
    do {
    wpid = waitpid(child_pid, &status, WNOHANG);
    printf("waited pid: %lu\n", wpid);
    sleep(1);
    } while(wpid == 0);
    WEXITSTATUS(status);
    
    printf("parent end\n");
    return 0;
  }

  //if (child_pid == 0) {
    close(fd[1]);
    memset(buf, '\0', sizeof(buf));
    printf("i am child, child_pid: %lu\n", child_pid);
    if (read(fd[0], buf, sizeof(buf)) < 0) {
      printf("child: read from parent error\n");
      exit(1);
    }
    close(fd[0]);
    printf("recv msg from parent: %s\n", buf);
  //}
  //if (child_pid == 0) {
    close(1);
    int n = 0;
    while(1) {
      if (n == 3) {
	printf("child tick over\n");
	break;
      }
      printf("child tick: %d\n", n++);
      sleep(1);
    }
  //}
  return 0;
}


