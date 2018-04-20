
#include <iostream>
#include <string>

// 1. function tempalte
template <class T>
T getMax(T t1, T t2)
{
  return (t1 > t2) ? t1 : t2;
}

template <typename T>
void swap(T &t1, T &t2)
{
  T tmp = t1;
  t1 = t2;
  t2 = tmp; 
}

/*
template <>
bool equals(int t1, int t2)
{
  return t1 == t2;
}
*/

// 2. class template
template <class T, class T2>
T2 Person<T, T2>::getEarings()
{
  return earnings;
}

template <>
void Patient<float>::seekTreatment()
{
  expense += 10;
}
