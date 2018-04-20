#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <malloc.h>
#include <sstream>
#include <utility>
#include <string.h>

#include "streambuf.h"
#include "mytemplate.h"

struct mid {
  int vlen;
  int vec[0];
};

void test_struct()
{
  struct mid mid;
  struct mid *mid2 = (struct mid *)malloc(sizeof(struct mid) + 2*sizeof(int));
  mid2->vlen = 1;
  //mid2.vec = new int[mid2.vlen];
  mid2->vec[0] = 10;
  mid2->vec[1] = 10;
 
  std::cout << " sizeof(mid) = " << sizeof(mid) << std::endl;
  std::cout<< " sizeof(mid2) = " << sizeof(mid2) << std::endl;
  std::cout<< " sizeof(*mid2) = " << sizeof(*mid2) << std::endl;
  std::cout<< " sizeof(mid2.vlen) = " << sizeof(mid2->vlen) << std::endl;
  std::cout<< " sizeof(mid2.vec) = " << sizeof(mid2->vec) << std::endl;
  std::cout<< " sizeof(mid2.vec[0]) = " << sizeof(mid2->vec[0]) << std::endl;
  std::cout<< " sizeof(mid2.vec[1]) = " << sizeof(mid2->vec[1]) << std::endl;  
  std::cout<< " sizeof(int) = " << sizeof(int) << std::endl;
  std::cout<< " sizeof(int *) = " << sizeof(int *) << std::endl;
  std::cout<< std::endl;
  free(mid2);
}

void test_streambuf()
{
  //char static_buf[1];
  size_t size = 80;
  void *ptr = ::operator new(size);
  PrebufferedStreambuf *sbufp =
	new PrebufferedStreambuf(reinterpret_cast<char*>(ptr), size);
  std::ostream os(sbufp);
  os << "this is a example of test PrebufferedStreambuf"
  ": public std::basic_streambuf<char, std::basic_string<char>::traits_type>";

  std::cout << " sbuf.str = " << sbufp->get_str() << std::endl;
  std::cout << " sbuf.size = " << sbufp->size()
	<< std::endl;
  delete sbufp;
  delete ptr;
}

class A {
public:
 A() {}
 ~A() {}
};

void test_stringstream()
{
  std::stringstream ss;
  std::stringbuf *pubf = ss.rdbuf();
  char *nptr = (char*)malloc(32*sizeof(char)); //new char[32];
  memset(nptr, '\0', 32);
  char *ptr = "1111111111111111";   
  
  memcpy(nptr, ptr, strlen(ptr) + 1);
  std::cout << "ptr = " << ptr << std::endl;
  std::cout << "len = " << strlen(ptr)<< std::endl;
  std::cout << "in_avail = " << pubf->in_avail() << std::endl;
 
  ss.rdbuf()->pubsetbuf(nptr, 32);
  std::cout << "in_avail = " << pubf->in_avail() << std::endl;
/*
  ss << "22222222";
  ss << "33333333";
  ss << "44444444";
*/
  std::cout << "in_avail = " << pubf->in_avail() << std::endl;
  //std::cout << "rdbuf = " << ss.rdbuf() << std::endl;
  std::cout << "rdbuf = " << ss.str() << std::endl;

  std::cout << "nptr = " << nptr << std::endl;
  /*
  int i = 0;
  for (i = 0; i < strlen(ptr); i++)
  pubf->snextc();
  free(nptr);
  nptr = NULL;
  //*/
  //ss.rdbuf()->pubsetbuf(NULL, 0);
  //ss.rdbuf()->pubsync();
  std::string ostr;
  ss >> ostr;
  std::cout << " ostr = " << ostr << std::endl;
  std::cout << "in_avail = " << pubf->in_avail() << std::endl;
  std::cout << "rdbuf = " << pubf << std::endl;
  
}

void test_template()
{
  int x = 1, y = 2;
  printf("getMax(%d,%d) = %d\n", x, y, getMax(x,y));
}

int main(int arc, char *argv[])
{
//  test_streambuf();
//  test_stringstream();
  test_template();
  return 0;
}
