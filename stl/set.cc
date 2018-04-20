
#include <set>
#include <iostream>
#include <unordered_set>

void print_set(std::set<int> &s)
{
  std::set<int>::iterator itr = s.begin();
  std::cout << "[";
  for (; itr != s.end(); itr++) {
    std::cout << *itr << ",";
  }
  std::cout << "]" << std::endl;
}

void print_set(std::unordered_set<int> &s)
{
  std::unordered_set<int>::iterator itr = s.begin();
  std::cout << "[";
  for (; itr != s.end(); itr++) {
    std::cout << *itr << ",";
  }
  std::cout << "]" << std::endl;
}

int main(int argc, char *argv[])
{
	//std::unordered_set<int> mset;
	std::set<int> mset;
	mset.insert(100);
	mset.insert(10);
	mset.insert(20);
	mset.insert(5);
	//print_set(mset);
	std::cout << mset << std::endl;
	return 0;
}
