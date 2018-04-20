
#include <iostream>
#include <map>
#include <sys/unistd.h>
#include <stdint.h>

void print_map(std::map<uint32_t, int64_t> &m)
{
  std::cout << "map size = " << m.size() << std::endl;
  std::map<uint32_t, int64_t>::const_iterator itr;
  for (itr = m.begin(); itr != m.end(); itr++) {
    std::cout << "< " << itr->first << ", " << itr->second << ">" << std::endl;
  }
}

int main(int argc, char *argv[])
{
  std::map<uint32_t, int64_t> map1;
  map1[10] = 10;
  map1[11] = 11;
  std::map<uint32_t, int64_t> map2(map1);
  std::map<uint32_t, int64_t> map3;
  map3 = map1;
  //map2[10] = 10;
  //map2[11] = 11;
  std::cout << "map1 == map2 ? " << (map1 == map2 ? "true" : "false")
	<< std::endl;
  //map1.clear();
  std::cout << "map1: " << std::endl;
  print_map(map1);
  std::cout << "map2: " << std::endl;
  print_map(map2);
  std::cout << "map3: " << std::endl;
  print_map(map3);
  return 0;
}
