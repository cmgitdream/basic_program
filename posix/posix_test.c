
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define NFS_NOT_SUPPORT 524
#define N 37

#define POSIX_MKDIR		0 
#define POSIX_OPENDIR		1 
#define POSIX_CLOSEDIR		2 
#define POSIX_READDIR		3
#define POSIX_RMDIR		4 
#define POSIX_CHDIR		5 
#define POSIX_REWINDDIR		6
#define POSIX_GETCWD		7
#define POSIX_GETDENTS		8
#define POSIX_CREATE		9
#define POSIX_CLOSE		10
#define POSIX_OPEN		11
#define POSIX_WRITE		12
#define POSIX_READ		13
#define POSIX_ACCESS		14
#define POSIX_RENAME		15
#define POSIX_UNLINK		16
#define POSIX_CHMOD		17
#define POSIX_FCHMOD		18
#define POSIX_CHOWN		19
#define POSIX_LINK		20
#define POSIX_SYMLINK		21
#define POSIX_READLINK		22
#define POSIX_FSYNC		23
#define POSIX_STAT		24
#define POSIX_FSTAT		25
#define POSIX_LSTAT		26
#define POSIX_TRUNCATE		27
#define POSIX_DUP		28
#define POSIX_DUP2		29
#define POSIX_LSEEK		30
#define POSIX_FSEEK		31
#define POSIX_FCNTL		32
#define POSIX_MMAP_MUNMAP	33
#define POSIX_MSYNC		34
#define POSIX_MLOCKALL_MUNLOCKALL	35
#define POSIX_IOCTL		36

int get_op_type(char *op)
{
  if (strcmp(op, "mkdir") == 0)
    return POSIX_MKDIR;
  if (strcmp(op, "opendir") == 0)
    return POSIX_OPENDIR;
  if (strcmp(op, "closedir") == 0)
    return POSIX_CLOSEDIR;
  if (strcmp(op, "readdir") == 0)
    return POSIX_READDIR;
  if (strcmp(op, "rmdir") == 0)
    return POSIX_RMDIR;
  if (strcmp(op, "chdir") == 0)
    return POSIX_CHDIR;
  if (strcmp(op, "rewinddir") == 0)
    return POSIX_REWINDDIR;
  if (strcmp(op, "getcwd") == 0)
    return POSIX_GETCWD;
  if (strcmp(op, "getdents") == 0)
    return POSIX_GETDENTS;
  if (strcmp(op, "create") == 0)
    return POSIX_CREATE;
  if (strcmp(op, "close") == 0)
    return POSIX_CLOSE;
  if (strcmp(op, "open") == 0)
    return POSIX_OPEN;
  if (strcmp(op, "write") == 0)
    return POSIX_WRITE;
  if (strcmp(op, "read") == 0)
    return POSIX_READ;
  if (strcmp(op, "access") == 0)
    return POSIX_ACCESS;
  if (strcmp(op, "rename") == 0)
    return POSIX_RENAME;
  if (strcmp(op, "unlink") == 0)
    return POSIX_UNLINK;
  if (strcmp(op, "chmod") == 0)
    return POSIX_CHMOD;
  if (strcmp(op, "fchmod") == 0)
    return POSIX_FCHMOD;
  if (strcmp(op, "link") == 0)
    return POSIX_LINK;
  if (strcmp(op, "symlink") == 0)
    return POSIX_SYMLINK;
  if (strcmp(op, "readlink") == 0)
    return POSIX_READLINK;
  if (strcmp(op, "fsync") == 0)
    return POSIX_FSYNC;
  if (strcmp(op, "stat") == 0)
    return POSIX_STAT;
  if (strcmp(op, "fstat") == 0)
    return POSIX_FSTAT;
  if (strcmp(op, "stat") == 0)
    return POSIX_STAT;
  if (strcmp(op, "lstat") == 0)
    return POSIX_LSTAT;
  if (strcmp(op, "truncate") == 0)
    return POSIX_TRUNCATE;
  if (strcmp(op, "dup") == 0)
    return POSIX_DUP;
  if (strcmp(op, "dup2") == 0)
    return POSIX_DUP2;
  if (strcmp(op, "lseek") == 0)
    return POSIX_LSEEK;
  if (strcmp(op, "fseek") == 0)
    return POSIX_FSEEK;
  if (strcmp(op, "fcntl") == 0)
    return POSIX_FCNTL;
  if (strcmp(op, "mmap") == 0)
    return POSIX_MMAP_MUNMAP;
  if (strcmp(op, "msync") == 0)
    return POSIX_MSYNC;
  if (strcmp(op, "mlockall") == 0)
    return POSIX_MLOCKALL_MUNLOCKALL;
  if (strcmp(op, "ioctl") == 0)
    return POSIX_IOCTL;
}

char *outdir = NULL;

int show_result(char *name, int support)
{
  if (!support) {
    printf("%16s:\t[ NOT_SUPPORT ]\n", name);
  } else {
    printf("%16s:\t[ SUPPORT     ]\n", name);
  }
  return 0;
}

int do_create(char *path)
{
  return open(path, O_CREAT|O_RDWR, 0644);
}

int do_create2(char *path)
{
  return open(path, O_CREAT|O_DIRECT|O_SYNC|O_RDWR, 0644);
}

int do_create3(char *path)
{
  return open(path, O_CREAT|O_ASYNC|O_RDWR, 0644);
}

int do_open(char *path)
{
  return open(path, O_RDWR, 0644);
}

int add_dir(char *parent, char *subdir)
{
  int len = strlen(parent) + strlen(subdir) + 2; // parent + '/' + subdir + '\0';
  char *child = (char *)malloc(sizeof(char) * len); 
  if (child == NULL)
    return -1;
  memset(child, '\0', len);
  snprintf(child, len, "%s/%s", parent, subdir);
  int r = mkdir(child, 0755);
  if (child)
    free(child);
  return r;
}

int test_mkdir(char *path)
{
  return mkdir(path, 0755);
}

int test_opendir(char *path)
{
  mkdir(path, 0755);
  DIR *dirp = opendir(path);
  if (dirp == NULL)
    return -1;
  return 0;
}

int test_closedir(char *path)
{
  mkdir(path, 0755);
  DIR *dirp = opendir(path);
  if (dirp == NULL)
    return -1;
  return closedir(dirp);
}

int test_readdir(char *path)
{
  mkdir(path, 0755);
  add_dir(path, "subdir1"); 
  DIR *dirp = opendir(path);
  if (dirp == NULL)
    return -1;
  struct dirent *dent = readdir(dirp);  
  if (dent == NULL)
    return -1;
  while(dent != NULL) {
    //printf("child = %s\n", dent->d_name);
    dent = readdir(dirp);
  }
  return 0;
}

int test_rmdir(char *path)
{
  mkdir(path, 0755);
  return rmdir(path);
}

int test_chdir(char *path)
{
  mkdir(path, 0755);
  char *subdir = "subdir2";
  int len = strlen(path) + strlen(subdir) + 2; // parent + '/' + subdir + '\0';
  char *child = (char *)malloc(sizeof(char) * len); 
  if (child == NULL)
    return -1;
  memset(child, '\0', len);
  snprintf(child, len, "%s/%s", path, subdir);
  mkdir(child, 0755);
  if (chdir(child) < 0);
    return -1;
  chdir(outdir);
  return 0;
}

int test_rewinddir(char *path)
{
  chdir(outdir);
  mkdir(path, 0755);
  add_dir(path, "subdir1"); 
  DIR *dirp = opendir(path);
  if (dirp == NULL)
    return -1;
  struct dirent *dent = readdir(dirp);  
  if (dent == NULL)
    return -1;
  int i = 0;
  while(dent != NULL) {
    if (i == 1) {
      rewinddir(dirp);
    }
    //printf("child = %s\n", dent->d_name);
    dent = readdir(dirp);
    i++;
  }
  return 0;
}

int test_getcwd(char *path)
{
  mkdir(path, 0755);
  chdir(outdir);
  char pwd[255];
  memset(pwd, '\0', 255);
  char *ptr = getcwd(pwd, 255);
  if (pwd == NULL) {
    chdir(outdir);
    return -1;
  }
  //printf("pwd = %s\n", ptr);
  chdir(outdir);
  return 0;
}

struct linux_dirent {
  long d_ino;
  off_t d_off;
  unsigned short d_reclen;
  char d_name[];
};

int test_getdents(char *path)
{
  mkdir(path, 0755);
  add_dir(path, "subdir3"); 
  int fd = open(path, O_RDONLY | O_DIRECTORY);
  struct linux_dirent *d;
  char buf[1024];
  int nread = 0;
  int i = 0;
  while(1) {
    nread = syscall(SYS_getdents, fd, buf, 1024);
    if (nread == -1)
      return -1;
    if (nread == 0)
      break;
    for (i = 0; i < nread;) {
      d = (struct linux_dirent *) (buf + i); 
      //printf("%s\n", d->d_name);
      i += d->d_reclen;
    }
  }
  return 0;
}

int test_create(char *path)
{
  return do_create(path);
}

int test_close(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  return close(fd);
}

int test_open(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if (close(fd) < 0)
    return -1;
  if ((fd = do_open(path)) < 0)
    return -1;
  return 0;
}

int test_write(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char buf[] = "test write line 1";
  int len = strlen(buf);
  int r = write(fd, buf, len);
  if (r < 0)
    return -1;
  //printf("write size = %d\n", r);
  close(fd);
  return 0;
}

int test_read(char *path)
{
  int fd = -1;
  if ((fd = do_create2(path)) < 0)
    return -1;
  char buf[] = "test read line 1";
  char buf2[32];
  memset(buf2, '\0', 32);
  int len = strlen(buf);
  int r = write(fd, buf, len);
  if (r < 0)
    return -1;
  r = read(fd, buf2, len);
  //printf("read size = %d\n", r);
  if (r < 0)
    return -1;
  //printf("buf2 = %s\n", buf2);
  close(fd);
  return 0;
}

int test_access(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  close(fd);
  if (access(path, R_OK) < 0)
    return -1;
  return 0;
}

int test_unlink(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  close(fd);
  if (unlink(path) < 0)
    return -1;
  return 0;
}

int test_chmod(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  close(fd);
  if (chmod(path, 0646) < 0)
    return -1;
  return 0;
}

int test_fchmod(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if (fchmod(fd, 0646) < 0)
    return -1;
  close(fd);
  return 0;
}

int test_rename(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char *suffix = ".rename";
  char *newpath;
  int len = strlen(path);
  int len2 = strlen(suffix);
  newpath = (char *) malloc(len + 1);
  if (newpath == NULL) {
    printf("%s: malloc failed\n", __func__);
    exit(1);
  }
  snprintf(newpath, len + len2 + 1, "%s%s", path, suffix);
  if(rename(path, newpath) < 0) {
    free(newpath);
    return -1;
  }
  free(newpath);
  return 0;
}

int test_link(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char *suffix = ".hardlink";
  char *newpath;
  int len = strlen(path);
  int len2 = strlen(suffix);
  newpath = (char *) malloc(len + 1);
  if (newpath == NULL) {
    printf("%s: malloc failed\n", __func__);
    exit(1);
  }
  snprintf(newpath, len + len2 + 1, "%s%s", path, suffix);
  if(link(path, newpath) < 0) {
    free(newpath);
    return -1;
  }
  free(newpath);
  return 0;
}

int test_symlink(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char *suffix = ".symlink";
  char *newpath;
  int len = strlen(path);
  int len2 = strlen(suffix);
  newpath = (char *) malloc(len + 1);
  if (newpath == NULL) {
    printf("%s: malloc failed\n", __func__);
    exit(1);
  }
  snprintf(newpath, len + len2 + 1, "%s%s", path, suffix);
  if(symlink(path, newpath) < 0) {
    free(newpath);
    return -1;
  }
  free(newpath);
  return 0;
}

int test_readlink(char *path)
{
  int fd = -1;
  if ((fd = do_create2(path)) < 0)
    return -1;
  char *suffix = ".hardlink";
  char *newpath;
  int len = strlen(path);
  int len2 = strlen(suffix);
  char buf[] = "test readlink line 1";
  char buf2[32];
  memset(buf2, '\0', 32);
  int len3 = strlen(buf);
  int r = write(fd, buf, len3);
  if (r < 0)
    return -1;
  close(fd);
  newpath = (char *) malloc(len + 1);
  if (newpath == NULL) {
    printf("%s: malloc failed\n", __func__);
    exit(1);
  }
  snprintf(newpath, len + len2 + 1, "%s%s", path, suffix);
  //if(link(path, newpath) < 0) {
  //  free(newpath);
  //  return -1;
  //}
  if(readlink(path, buf2, len3) < 0) {
    free(newpath);
    return -1;
  }
  free(newpath);
  return 0;
}

int test_fsync(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char buf[] = "test fsync line 1";
  int len = strlen(buf);
  int r = write(fd, buf, len);
  if (r < 0)
    return -1;
  if (fsync(fd) < 0)
    return -1;
  close(fd);
  return 0;
}

int test_stat(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if (close(fd) < 0)
    return -1;
  struct stat st;
  if (stat(path, &st) < 0)
    return -1;
  return 0;
}

int test_fstat(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if (close(fd) < 0)
    return -1;
  fd = do_open(path);
  struct stat st;
  if (fstat(fd, &st) < 0)
    return -1;
  return 0;
}

int test_lstat(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if (close(fd) < 0)
    return -1;
  struct stat st;
  if (lstat(path, &st) < 0)
    return -1;
}

int test_truncate(char *path)
{
  int fd = -1;
  if ((fd = do_create(path)) < 0) {
    printf("%s: create %s failed\n", __func__, path);
    return -1;
  }
  if (close(fd) < 0) {
    printf("%s: close %s failed\n", __func__, path);
    return -1;
  }
  int size = 10;
  // nfs-ganesha + rgwfile support, but not implement truncate
  // wo we consider it not support by check size
  if (truncate(path, size) < 0) {
    printf("%s: truncate failed\n", __func__);
    //errno = NFS_NOT_SUPPORT;
    return -1;
  }
  struct stat st;
  if (stat(path, &st) < 0) {
    printf("%s: stat failed\n", __func__);
    //errno = NFS_NOT_SUPPORT;
    return -1;
  }
  //printf("%s: st_size = %d\n", __func__, st.st_size);
  if (st.st_size != size) {
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  return 0;
}

int test_dup(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if ((fd2 = dup(fd)) < 0)
    return -1;
  return 0;
}

int test_dup2(char *path)
{
  int fd = -1;
  int fd2 = -1;
  
  if ((fd = do_create(path)) < 0)
    return -1;
  char *suffix = ".fd2";
  int len = strlen(path);
  int len2 = strlen(suffix);
  char *newpath = (char *) malloc(len + 1);
  if (newpath == NULL) {
    printf("%s: malloc failed\n", __func__);
    exit(1);
  }
  snprintf(newpath, len + len2 + 1, "%s%s", path, suffix);
  if ((fd2 = do_create(newpath)) < 0)
    return -1;
  if (dup2(fd, fd2) < 0)
    return -1;
  return 0;
}

int test_lseek(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  int offset = 100;
  if (lseek(fd, offset, SEEK_SET) < 0)
    return -1;
  char *data = "lseek data content";
  int len = strlen(data);
  if (write(fd, data, len) < 0) {
    printf("%s: wirte after lseek failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  struct stat st;
  if (fstat(fd, &st) < 0) {
    printf("%s: fstat after lseek failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  if (st.st_size != (offset + len)) {
    printf("%s: update filesize after lseek & write failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  close(fd);
  return 0;
}

int test_fseek(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  if (close(fd) < 0)
    return -1;
  FILE *fp1 = fopen(path, "r+");
  if (fp1 == NULL) {
    return -1;
  }
  
  int offset = 100;
  if (fseek(fp1, offset, SEEK_SET) < 0)
    return -1;
  char *data = "fseek data content";
  int len = strlen(data);
  if (fwrite((void *)data, len, 1, fp1) < 0) {
    printf("%s: fwirte after fseek failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  if (fclose(fp1) < 0) {
    printf("%s: fclose after fseek & fwrite failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  struct stat st;
  if (stat(path, &st) < 0) {
    printf("%s: stat after fseek & fwrite failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  if (st.st_size != (offset + len)) {
    printf("%s: update filesize after lseek & write failed\n", __func__);
    errno = NFS_NOT_SUPPORT;
    return -1;
  }
  return 0;
}

// todo: more logic test for
// 1. dup fd: F_DUPFD
// 2. set fd: F_GETFD, F_SETFD
// 3. flags:  F_GETFL, F_SETFL
// 4. signal: F_GETOWN, F_SETOWN
// 5. flock:  F_GETLK, F_SETLK
int test_fcntl(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  int flags = 0;
  if ((flags = fcntl(fd, F_GETFL)) < 0)
    return -1;
  if (fcntl(fd, F_SETFL, flags|O_NONBLOCK) < 0)
    return -1;
  close(fd);
  return 0;
}

int test_mmap_munmap(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create2(path)) < 0)
    return -1;
  char buf[] = "mmap write line 1";
  int len = strlen(buf) + 1;
  int r = write(fd, buf, len);
  if (r < 0)
    return -1;
  if (fsync(fd) < 0)
    return -1;
  struct stat st;
  if (fstat(fd, &st) < 0)
    return -1;
  char *ptr = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    printf("%s: mmap file %s failed\n", __func__, path);
    return -1;
  }
  close(fd);
  //printf("ptr = %s\n", ptr);
  ptr[0] = 'M';
  //printf("ptr = %s\n", ptr);
  char *buf2 = (char *)malloc(sizeof(char) * len);
  if (buf2 == NULL)
    return -1;
  
  if (munmap(ptr, st.st_size) == -1) {
    printf("%s: munmap failed\n", __func__); 
    return -1;
  }

  fd = do_open(path);
  read(fd, buf2, len);
  close(fd);

  //printf("buf2 = %s\n", buf2);
   
  return 0;
}

int test_msync(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char buf[] = "msync write line 1";
  int len = strlen(buf) + 1;
  int r = write(fd, buf, len);
  if (r < 0)
    return -1;
  if (fsync(fd) < 0)
    return -1;
  struct stat st;
  if (fstat(fd, &st) < 0)
    return -1;
  char *ptr = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    printf("%s: mmap file %s failed\n", __func__, path);
    return -1;
  }
  close(fd);
  //printf("ptr = %s\n", ptr);
  int i = 0;
  for (i = 0; i < st.st_size; i++) {
    ptr[i] = toupper(ptr[i]);
  }
  //printf("ptr = %s\n", ptr);
  if (msync(ptr, st.st_size, MS_SYNC) < 0)
    return -1;
   
  if (munmap(ptr, st.st_size) == -1) {
    return -1;
  }
  return 0;
}


int test_mlockall_munlockall(char *path)
{
  int fd = -1;
  int fd2 = -1;
  if ((fd = do_create(path)) < 0)
    return -1;
  char buf[] = "mlocall_munlockall write line 1";
  int len = strlen(buf) + 1;
  int r = write(fd, buf, len);
  if (r < 0)
    return -1;
  if (fsync(fd) < 0)
    return -1;
  struct stat st;
  if (fstat(fd, &st) < 0)
    return -1;
  char *ptr = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    printf("%s: mmap file %s failed\n", __func__, path);
    return -1;
  }
  close(fd);
  //printf("ptr = %s\n", ptr);
  if (mlockall(MCL_CURRENT) < 0) {
    printf("%s: mlocakall failed\n", __func__);
    return -1;
  }
  int i = 0;
  for (i = 0; i < st.st_size; i++) {
    ptr[i] = toupper(ptr[i]);
  }
  //printf("ptr = %s\n", ptr);
  if (msync(ptr, st.st_size, MS_SYNC) < 0)
    return -1;

  if (munlockall() < 0) {
    printf("%s: munlockall failed\n", __func__);
    return -1;
  }
   
  if (munmap(ptr, st.st_size) == -1) {
    return -1;
  }
  return 0;
}


//typedef int (*test_func)(char *);
struct func_ent {
  char *name;
  int (*func)(char *);
  char *para;
};

const struct func_ent test_cases[N] = {
  [POSIX_MKDIR] = {
    .name = "mkdir",
    //.func = NULL,
    .func = test_mkdir,
    .para = "mkdir1",
  }, 
  [POSIX_OPENDIR] = {
    .name = "opendir",
    //.func = NULL,
    .func = test_opendir,
    .para = "opendir1",
  }, 
  [POSIX_CLOSEDIR] = {
    .name = "closedir",
    .func = NULL,
    .func = test_opendir,
    .para = "closedir1",
  }, 
  [POSIX_READDIR] = {
    .name = "readdir",
    .func = NULL,
    .func = test_readdir,
    .para = "readdir1",
  }, 
  [POSIX_RMDIR] = {
    .name = "rmdir",
    .func = NULL,
    .func = test_rmdir,
    .para = "rmdir1",
  }, 
  [POSIX_CHDIR] = {
    .name = "chdir",
    .func = NULL,
    .func = test_chdir,
    .para = "chdir1",
  }, 
  [POSIX_REWINDDIR] = {
    .name = "rewinddir",
    .func = NULL,
    .func = test_rewinddir,
    .para = "rewinddir1",
  }, 
  [POSIX_GETCWD] = {
    .name = "getcwd",
    .func = NULL,
    .func = test_getcwd,
    .para = "getcwddir1",
  }, 
  [POSIX_GETDENTS] = {
    .name = "getdents",
    .func = test_getdents,
    .para = "getdentsdir1",
  }, 
  [POSIX_CREATE] = {
    .name = "create",
    .func = NULL,
    .func = test_create,
    .para = "createfile1",
  }, 
  [POSIX_CLOSE] = {
    .name = "close",
    .func = NULL,
    .func = test_close,
    .para = "closefile1",
  }, 
  [POSIX_OPEN] = {
    .name = "open",
    .func = NULL,
    .func = test_open,
    .para = "openfile1",
  }, 
  [POSIX_WRITE] = {
    .name = "write",
    .func = NULL,
    .func = test_write,
    .para = "writefile1",
  }, 
  [POSIX_READ] = {
    .name = "read",
    .func = NULL,
    .func = test_read,
    .para = "readfile1",
  }, 
  [POSIX_ACCESS] = {
    .name = "access",
    .func = NULL,
    .func = test_access,
    .para = "accessfile1",
  }, 
  [POSIX_RENAME] = {
    .name = "rename",
    .func = NULL,
    .func = test_rename,
    .para = "renamefile1",
  }, 
  [POSIX_UNLINK] = {
    .name = "unlink",
    .func = NULL,
    .func = test_unlink,
    .para = "unlinkfile1",
  }, 
  [POSIX_CHMOD] = {
    .name = "chmod",
    .func = NULL,
    .func = test_chmod,
    .para = "chmodfile1",
  }, 
  [POSIX_FCHMOD] = {
    .name = "fchmod",
    .func = NULL,
    .func = test_fchmod,
    .para = "fchmodfile1",
  }, 
  [POSIX_LINK] = {
    .name = "link",
    .func = NULL,
    .func = test_link,
    .para = "linkfile1",
  }, 
  [POSIX_SYMLINK] = {
    .name = "symlink",
    .func = NULL,
    .func = test_symlink,
    .para = "symlinkfile1",
  }, 
  [POSIX_READLINK] = {
    .name = "readlink",
    .func = NULL,
    .func = test_readlink,
    .para = "readlinkfile1",
  }, 
  [POSIX_FSYNC] = {
    .name = "fsync",
    .func = NULL,
    .func = test_fsync,
    .para = "fsyncfile1",
  }, 
  [POSIX_STAT] = {
    .name = "stat",
    .func = NULL,
    .func = test_stat,
    .para = "statfile1",
  }, 
  [POSIX_FSTAT] = {
    .name = "fstat",
    .func = NULL,
    .func = test_fstat,
    .para = "fstatfile1",
  }, 
  [POSIX_LSTAT] = {
    .name = "lstat",
    .func = NULL,
    .func = test_lstat,
    .para = "lstatfile1",
  }, 
  [POSIX_TRUNCATE] = {
    .name = "truncate",
    .func = NULL,
    .func = test_truncate,
    .para = "truncatefile1",
  }, 
  [POSIX_DUP] = {
    .name = "dup",
    .func = NULL,
    .func = test_dup,
    .para = "dupfile1",
  }, 
  [POSIX_DUP2] = {
    .name = "dup2",
    .func = NULL,
    .func = test_dup2,
    .para = "dup2file1",
  }, 
  [POSIX_LSEEK] = {
    .name = "lseek",
    .func = NULL, 
    .func = test_lseek,
    .para = "lseekfile1",
  }, 
  [POSIX_FSEEK] = {
    .name = "fseek",
    .func = NULL,
    .func = test_fseek,
    .para = "fseekfile1",
  }, 
  [POSIX_FCNTL] = {
    .name = "fcntl",
    .func = NULL,
    .func = test_fcntl,
    .para = "fcntlfile1",
  }, 
  [POSIX_MMAP_MUNMAP] = {
    .name = "mmap",
    .func = NULL,
    .func = test_mmap_munmap,
    .para = "mmap_munmap_file1",
  }, 
/*
  [POSIX_MUNMAP] = {
    .name = "munmap",
    .func = NULL, //test_munmap,
    .para = "munmapfile1",
  }, 
*/
  [POSIX_MSYNC] = {
    .name = "msync",
    .func = NULL,
    .func = test_msync,
    .para = "msyncfile1",
  }, 
  [POSIX_MLOCKALL_MUNLOCKALL] = {
    .name = "mlockall",
    .func = NULL,
    .func = test_mlockall_munlockall,
    .para = "mlockall_munlockall_file1",
  }, 
/*
  [POSIX_MUNLOCKALL] = {
    .name = "munlockall",
    .func = NULL, //test_munlockall,
    .para = "munlockallfile1",
  }, 
*/
  [POSIX_IOCTL] = {
    .name = "ioctl",
    .func = NULL,
    //.func = test_ioctl,
    .para = "ioctlfile1",
  }, 
};

void usage()
{
  printf("./posix_test </path/to/output_dir> <op_name>\n");
  printf("op_name = mkdir|create|read|write ... | all\n");
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    usage();
    exit(1);
  }
  outdir = argv[1];
  char *op_name = argv[2];
  mkdir(outdir, 0755);
  char *path = "linkfile1";
  chdir(outdir);
  int i = 0;
  int ret = 0;
  int support = 0;
  if (strcmp(op_name, "all") == 0) {
  for (; i < N; i++) {
    errno = 0;
    if (test_cases[i].func == NULL)
     continue;
    ret = test_cases[i].func(test_cases[i].para);
    support = (errno != NFS_NOT_SUPPORT) ? 1 : 0;
    //printf("%s: errno = %d, support = %d, ret = %d\n", test_cases[i].name, errno, support, ret);
    show_result(test_cases[i].name, support);
  }
  } else {
    int op = get_op_type(op_name);
    errno = 0;
    if (test_cases[op].func == NULL)
     return 0;
    ret = test_cases[op].func(test_cases[op].para);
    support = (errno != NFS_NOT_SUPPORT) ? 1 : 0;
    //printf("%s: errno = %d, support = %d, ret = %d\n", test_cases[op].name, errno, support, ret);
    show_result(test_cases[op].name, support);
  }
  return 0; 
}
