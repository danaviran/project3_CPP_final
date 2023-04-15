#include <malloc.h>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>

using std::vector;
using std::string;

struct KeyHash {
    unsigned long operator()(const K& key) const
    {
      return reinterpret_cast<unsigned long>(key) % TABLE_SIZE;
    }
};

class HashMap
{
  const int capacity = 20;
  int size = 0;
  int count = 0;

  struct HashNode** arr;
  struct HashNode* dummy;
  F hashFunc;

  class HashNode {
   public:
    HashNode(const K &key, const V &value) :
        key(key), value(value), next(NULL) {
    }

    K getKey() const {
      return key;
    }

    V getValue() const {
      return value;
    }

    void setValue(V value) {
      HashNode::value = value;
    }

    HashNode *getNext() const {
      return next;
    }

    void setNext(HashNode *next) {
      HashNode::next = next;
    }

   private:
    // key-value pair
    K key;
    V value;
    // next bucket with the same key
    HashNode *next;
  };


 public:
  HashMap() {
    // construct zero initialized hash table of size
    table = new HashNode<K, V> *[TABLE_SIZE]();
  }

  ~HashMap() {
    // destroy all buckets one by one
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode<K, V> *entry = table[i];
        while (entry != NULL) {
            HashNode<K, V> *prev = entry;
            entry = entry->getNext();
            delete prev;
          }
        table[i] = NULL;
      }
    // destroy the hash table
    delete [] table;
  }

  HashMap(vector<int> keys, vector<string> values);


  HashMap* operator=(HashMap* other);

  int get_capacity();

  int get_size();

  bool is_empty();

  int bucket_size(K key);

  int bucket_index(K key);



  bool get(const K &key, V &value) {
    unsigned long hashValue = hashFunc(key);
    HashNode<K, V> *entry = table[hashValue];

    while (entry != NULL) {
        if (entry->getKey() == key) {
            value = entry->getValue();
            return true;
          }
        entry = entry->getNext();
      }
    return false;
  }

  void put(const K &key, const V &value) {
    unsigned long hashValue = hashFunc(key);
    HashNode<K, V> *prev = NULL;
    HashNode<K, V> *entry = table[hashValue];

    while (entry != NULL && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
      }

    if (entry == NULL) {
        entry = new HashNode<K, V>(key, value);
        if (prev == NULL) {
            // insert as first bucket
            table[hashValue] = entry;
          } else {
            prev->setNext(entry);
          }
      } else {
        // just update the value
        entry->setValue(value);
      }
  }

  void erase(const K &key) {
    unsigned long hashValue = hashFunc(key);
    HashNode<K, V> *prev = NULL;
    HashNode<K, V> *entry = table[hashValue];

    while (entry != NULL && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
      }

    if (entry == NULL) {
        // key not found
        return;
      }
    else {
        if (prev == NULL) {
            // remove first bucket of the list
            table[hashValue] = entry->getNext();
          } else {
            prev->setNext(entry->getNext());
          }
        delete entry;
      }
  }

// Function to add key value pair
  void insert(int key, int V)
  {
    auto* temp = (struct HashNode*) malloc (sizeof(struct HashNode));
    temp->key = key;
    temp->value = V;

    // Apply hash function to find
    // index for given key
    int hashIndex = key % capacity;

    // Find next free space
    while (arr[hashIndex] != nullptr && arr[hashIndex]->key != key
           && arr[hashIndex]->key != -1)
      {
        hashIndex++;
        hashIndex %= capacity;
      }

    // If new node to be inserted
    // increase the current size
    if (arr[hashIndex] == nullptr || arr[hashIndex]->key == -1)
      size++;

    arr[hashIndex] = temp;
  }

// Function to delete a key value pair
  int deleteKey(int key)
  {
    // Apply hash function to find
    // index for given key
    int hashIndex = key % capacity;

    // Finding the node with given
    // key
    while (arr[hashIndex] != nullptr) {
        // if node found
        if (arr[hashIndex]->key == key) {
            // Insert dummy node here
            // for further use
            arr[hashIndex] = dummy;

            // Reduce size
            size--;

            // Return the value of the key
            return 1;
          }
        hashIndex++;
        hashIndex %= capacity;
      }

    // If not found return null
    return 0;
  }

// Function to search the value
// for a given key
  int find(int key)
  {
    // Apply hash function to find
    // index for given key
    int hashIndex = (key % capacity);

    int counter = 0;

    // Find the node with given key
    while (arr[hashIndex] != nullptr) {

        int counter = 0;
        // If counter is greater than
        // capacity
        if (counter++ > capacity)
          break;

        // If node found return its
        // value
        if (arr[hashIndex]->key == key)
          return arr[hashIndex]->value;

        hashIndex++;
        hashIndex %= capacity;
      }

    // If not found return
    // -1
    return -1;
  }

  class Hash_Map_Iterator
  {
    typedef typename constness_traits_t::pointer pointer;
    typedef typename constness_traits_t::reference reference;
    reference operator*()const {return (*this->container_)[this->current_].value_;}
    pointer operator->()const {return &(operator*());}
  };
};

