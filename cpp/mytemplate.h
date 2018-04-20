
#include <iostream>
#include <string>

// 1. function tempalte
template <class T>
T getMax(T t1, T t2);

template <typename T>
void swap(T t1, T t2);

/*
template <>
bool equals(int t1, int t2);
*/

// 2. class template
template <class T, class T2>
class Person {
private:
  T gift;
  std::string name;
  T2 earnings;
public:
  Person(T _gift, std::string _name):gift(_gift), name(_name) {}
  ~Person() {}
  T2 getEarings(); 
};

template <>
class Patient <float> {
private:
  float expense;
  std::string name;
public:
  Patient(std::string _name):name(_name) {}
  ~Patient() {}
  void seekTreatment(); 
};

#include "mytemplate.cc"
