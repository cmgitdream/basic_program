
#include <list>
#include <map>
#include <iostream>
#include <inttypes.h>
#include <assert.h>

using namespace std;

#define LRU_CAPACITY 4

/*
 * eliminate the last, and push front if hit or add new one
 *
 * if capacity = 4:
 * 1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6
 * list front -> end:
 * 1
 * 2 1    
 * 3 2 1 
 * 4 3 2 1
 * 2 4 3 1
 * 1 2 4 3
 * 5 1 2 4
 * 6 5 1 2
 * 2 6 5 1
 * 1 2 6 5
 * 2 1 6 5
 * 3 2 1 6
 * 7 3 2 1
 * 6 7 3 2
 * 3 6 7 2
 * 2 3 6 7
 * 1 2 3 6
 * 2 1 3 6
 * 3 2 1 6
 * 6 3 2 1
 * 
 */

// total = hit + out + LRUCache.capacity
struct lru_stats {
  uint64_t m_acc_total;
  uint64_t m_acc_hit;
  uint64_t m_acc_out;
  lru_stats() : m_acc_total(0), m_acc_hit(0), m_acc_out(0) {}
};

template <class K, class V>
class lru_node {
public:
  K m_key;
  V m_value;
  lru_node(K key, V val) : m_key(key), m_value(val) {}
};

template <class K, class V>
class LRUCache {
private:
  list<lru_node<K,V>*> m_list;
  map<K, lru_node<K,V>*> m_map;
  int m_capacity;
public:
  lru_stats m_stats;
  LRUCache(int capacity = LRU_CAPACITY) : m_capacity(capacity) {}
  ~LRUCache();
  int set(K key, V value);
  lru_node<K,V> * get(K key);
  const list<lru_node<K,V>*> & get_list();
  int get_capacity();
  int get_size();
};

template <class K, class V>
ostream & operator<< (ostream &out, LRUCache<K, V> &lru)
{
  out << "lru: front->end [";
  const list<lru_node<K,V>*> & mylist = lru.get_list();
  typename list<lru_node<K,V>*>::const_iterator litr = mylist.begin();
  for (; litr != mylist.end(); litr++) {
    out << (*litr)->m_key << ",";
  }
  out << "]";
  out << ", total:" << lru.m_stats.m_acc_total;
  out << ", hit:" << lru.m_stats.m_acc_hit;
  out << ", nohit:" << lru.m_stats.m_acc_out;
  return out;
}

template <class K, class V>
LRUCache<K, V>::~LRUCache()
{
  typename map<K, lru_node<K,V>*>::iterator itr;
  while (!m_list.empty()) {
      lru_node<K,V> *last_node = m_list.back();
      assert(last_node);

      itr = m_map.find(last_node->m_key);
      assert(itr != m_map.end());

      m_map.erase(itr);
      m_list.pop_back();

      delete last_node;
      last_node = NULL;
  }
}

template <class K, class V>
int LRUCache<K, V>::set(K key, V value)
{
  typename map<K, lru_node<K,V>*>::iterator itr = m_map.find(key);
  typename map<K, lru_node<K,V>*>::iterator itr2;
  typename list<lru_node<K,V>*>::iterator litr;
  typename list<lru_node<K,V>*>::iterator litr2;
  lru_node<K, V> * node;
  if (itr != m_map.end()) {
    node = itr->second;
    // todo: use list_head double link to replace std::list
    for (litr = m_list.begin(); litr != m_list.end(); litr++) {
      if ((*litr)->m_key == key)
	break;
    }
    assert(litr != m_list.end());
    // move hit node to head(head points to hit node)
    m_list.splice(m_list.begin(), m_list, litr);
    m_stats.m_acc_total++; 
    m_stats.m_acc_hit++;
  } else {
    lru_node<K,V> *new_node = new lru_node<K,V>(key, value);
    if (m_list.size() >= m_capacity) {
      lru_node<K,V> *last_node = m_list.back();
      assert(last_node);

      itr2 = m_map.find(last_node->m_key);
      assert(itr2 != m_map.end());

      m_map.erase(itr2);
      m_list.pop_back();

      delete last_node;
      last_node = NULL;
      m_stats.m_acc_out++;
    }
    m_list.push_front(new_node);
    m_map[key] = new_node;
    m_stats.m_acc_total++; 
  }
  return 0;
}

template <class K, class V>
lru_node<K,V> * LRUCache<K, V>::get(K key)
{
  typename map<K, lru_node<K,V>*>::iterator itr = m_map.find(key);
  if (itr != m_map.end())
    return itr->second;
  return NULL;
}

template <class K, class V>
const list<lru_node<K,V>*> & LRUCache<K, V>::get_list()
{
  return m_list;
}

template <class K, class V>
int LRUCache<K, V>::get_capacity()
{
  return m_capacity;
}

template <class K, class V>
int LRUCache<K, V>::get_size()
{
  return m_list.size();
}

int main(int argc, char *argv[])
{
  int ids[] = {1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6};  
  LRUCache<int, int> lru;
  int i = 0;
  int size = sizeof(ids)/sizeof(int);
  for (; i < size; i++) {
    lru.set(ids[i], ids[i]);
    cout << lru << endl;
  }
  return 0;
}
