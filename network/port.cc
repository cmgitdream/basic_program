
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include <regex.h>
#include <set>

int split_str(char *text, char *delim, char **res, size_t *count)
{
  if (text == NULL || delim == NULL)
    return -1;
  char *rs = NULL;
  size_t len = 0;
  int i = 0;
  do {
    rs = strsep(&text, delim); 
    if (rs) {
      res[i] = rs;
      i++; 
    }
  } while (rs);
  *count = i;
  return 0;
}

int reg_match(char *buf, char *pattern, int *port)
{
  regex_t  reg;
  regmatch_t pmatch[4];
  size_t nmatch = 4;
  int cflags = REG_EXTENDED|REG_NEWLINE;
  int eflags = 0;
  char ebuf[128];
  int i = 0;
  int ret;

  // generate regex by pattern
  ret = regcomp(&reg, pattern, cflags); 
  if (ret < 0) {
    regerror(ret, &reg, ebuf, sizeof(ebuf));
    return -1;
  }
 
  // only math the first pattern
  ret = regexec(&reg, buf, nmatch, pmatch, eflags); 
  if (ret != 0) {
    regerror(ret, &reg, ebuf, sizeof(ebuf));
    return -1;
  }
  
  std::string s(buf);
  const char *numptr = NULL;
  size_t start;
  size_t end;
  i = 1; // just need ([0-9A-F]{4})
  start = pmatch[i].rm_so;
  end = pmatch[i].rm_eo;
  numptr = s.substr(start, end-start).c_str();
  *port = strtoul(numptr, NULL, 16); 
  //printf("%s=%d\n", numptr, *port);
  regfree(&reg); 
  return 0;
}

int read_net_port_one_line(FILE *fp, std::set<int> &ports, bool *eof)
{
  size_t buf_len = 256;
  char buf[256];
  char *line = NULL;
  size_t count = 0;
  int n;
  int ret;

  //n = read(fd, buf, buf_len); 
  memset(buf, 0, buf_len);
  line = fgets(buf, buf_len - 1, fp);
  if (!line) {
    if (ferror(fp)) {
      ret = -errno;
      return ret;
    } else {
      *eof = true;
    }
  }
  printf("%s", buf);

  // cat /proc/net/tcp result : rem_addr shoud be 00000000
  // this port is selected as binded port
  char pattern[] = ":([0-9A-F]{4}) [0]{8}:";
  size_t i = 0; // skip first line -- title
  int port = 0;

  if (reg_match(buf, pattern, &port) == 0)
    ports.insert(port);

  return 0;
}

int read_net_ports(char *file, std::set<int> &ports)
{
  size_t nread = 0;
  FILE *fp;
  bool eof = false;
  int ret;
  fp = fopen(file, "r"); 
  if (!fp) {
    printf("%s, open failed, error = %d\n", __func__, errno);
    return -1;
  }

  do {
    read_net_port_one_line(fp, ports, &eof);  
  } while(!eof);

  printf("\n");
  fclose(fp);
  return 0;
}


int read_tcp_port(std::set<int> &ports)
{
  char file[] = "/proc/net/tcp";
  return read_net_ports(file, ports);
}

int read_udp_port(std::set<int> &ports)
{
  char file[] = "/proc/net/udp";
  return read_net_ports(file, ports);
}


int main(int argc, char *argv[])
{
  std::set<int> ports;
  read_tcp_port(ports);
  read_udp_port(ports);
  std::set<int>::iterator itr = ports.begin();
  for (; itr != ports.end(); itr++) {
    printf("%d\n", *itr);
  }
  return 0;
}
