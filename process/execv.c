
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  //pid_t pid = fork();
  // in child process
  int fd[2];
  int ret = pipe(fd);

  if (daemon(1, 1) < 0) {
    printf(" daemonize error\n");
    exit(1);
  }

  if (fork() == 0) {
    printf(" CHILD {\n");
    char **args = NULL;
    char *command = NULL;
    printf(" command %s %s\n", argv[1], argv[2]);
    if (argc > 1)
	command=argv[1];
    if (argc > 2) 
	args = &argv[2];
    execv(command, args);
    //printf(" command %s execv() failed, error code %d(%s)\n", command, errno, strerror(errno));
    printf(" CHILD }\n");
  } else {
    printf(" PARENT {\n");
    printf("parent process finish");
    printf(" PARENT }\n");
  }
  return 0;
}
