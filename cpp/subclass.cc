
#include <iostream>
#include <string>
#include <stdint.h>

typedef unsigned int version_t;
using std::string;
using std::cout;
using std::endl;
using std::cerr;

class Map {
public:
  version_t epoch;
  Map():epoch(0){}
  ~Map() {}
};

class Manager {
public:
  Map m_map;
  string m_name;
  Manager(string name): m_name(name) {}
  ~Manager() {}
};

int main(int argc, char *argv[])
{
  Manager me("myname");
  cout << "Manager: epoch = " << me.m_map.epoch << endl;
}
