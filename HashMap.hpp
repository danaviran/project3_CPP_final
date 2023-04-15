#ifndef HASHMAP_EX6
#define HASHMAP_EX6

#include <vector>
#include <stdexcept>
#include <algorithm>

#define INIT_CAPACITY 16
#define INVALID_MSG "Error: Invalid argument"
#define KEY_NOT_FOUND "Error: Key not found"
#define VECTOR_LENGTH "Error: Vectors length is not the same"
#define LOAD_FACTOR_MAX 0.75
#define LOAD_FACTOR_MIN 0.25
#define INIT 0
#define ERASE_LAST_INIT 1
#define ERASE_LAST_INDICATION 0
#define MULT 2
#define DIV 0.5
#define HASH_HELP 1


/**
 * HashMap class
 * Represents an unordered hash-map container of keyT-ValueT generic template
 * @tparam KeyT
 * @tparam ValueT
 */
template<class KeyT, class ValueT>
class HashMap {

  // Private Members
  typedef std::vector<std::pair<KeyT, ValueT>> bucket;
  bucket *buckets; // Array of buckets
  int _size=INIT; // Num of pairs in buckets array
  int _capacity = INIT_CAPACITY; // Num of buckets

  // Private helper functions

  /**
 * Hash Function that works on modulo
 * Gets a key of unknown hashable type and returns an integer that represents
 * the number of bucket to insert it to.
 * @param key
 */
  int hash_func (const KeyT &key) const;

  /**
   * Rehash func
   * deletes current buckets array and allocates new array in new size.
   * @param mult double to multiply by
   */
  void rehash (double mult);

  /**
   * Update the capacity of the array - multiply by MULT or divide by DIV
   * @param is_grow bool that indicates whether to mult or div
   * calls the operation function - hash_func
   */
  void update_capacity (bool is_grow);


 public:
  // Constructors and Destructor
  HashMap () : // Default
  _size (0), _capacity (INIT_CAPACITY){buckets = new bucket[INIT_CAPACITY];}

  /**
   * Main Constructor
   * Gets a vector of KeyT type and a vector of ValueT type and inserts the
   * key-value pairs by order to the buckets array
   * @param keyVec
   * @param valVec
   */
  HashMap (const std::vector<KeyT> &keyVec, const std::vector<ValueT> &valVec);

  /**
   * Copy Constructor
   * @param other
   */
  HashMap (const HashMap<KeyT, ValueT> &other);

  virtual ~HashMap (){delete[] buckets;} // Simple Destructor

  /**
   * Operator=
   * @param other
   * @return new HashMap, a deep copy of other
   */
  HashMap &operator= (const HashMap<KeyT, ValueT> &other);

  // Getters and Checkers
  int size () const { return _size; }
  int capacity () const { return _capacity; }
  int bucket_size (const KeyT &key);
  int bucket_index (const KeyT &key) const;
  double get_load_factor () const {return (double) _size / _capacity;}
  bool contains_key (const KeyT &key) const;
  bool empty () const;

  // Operations

  /**
   * Insert Function
   * Inserts a pair of key-value to bucket array, if array does not contain key
   * @param key
   * @param value
   * @return true upon success
   */
  bool insert (const KeyT &key, const ValueT &value);

  /**
   * Erase pair
   * @param key
   * @return true upon success of operation
   */
  virtual bool erase (const KeyT &key);

  /**
   * Clear all pairs in buckets array
   */
  void clear ();

  // Operators

  /**
   * At operator
   * Raises exceptions if does not exist
   * @param key
   * @return ValueT if exists
   */
  ValueT &at (const KeyT &key);
  const ValueT &at (const KeyT &key) const; // const version

  /**
   * Operator []
   * @param key
   * @return ValueT if exists
   */
  ValueT &operator[] (const KeyT &key);
  const ValueT &operator[] (const KeyT &key) const; // const version

  /**
   * Operator ==
   * @param other
   * @return true if have same pairs inside array, not necessarily in the same
   * order or capacity
   */
  bool operator== (const HashMap &other) const;
  bool operator!= (const HashMap &other) const;

  // Begin & End functions
  class ConstIterator;
  using const_iterator = ConstIterator;
  const_iterator begin () const {return ConstIterator (*this, true);}
  const_iterator cbegin () const {return begin ();}
  const_iterator end () const {return ConstIterator (*this, false);}
  const_iterator cend () const {return end ();}

  // Nested class - ConstIterator
  class ConstIterator {
    // Privates
    int bucket_ind, pair_ind; // Indexes of Iterator
    const HashMap<KeyT, ValueT> & hash_map; // hash_map to iterate on
   public:
    // Iterator traits:
    typedef std::pair<KeyT, ValueT> value_type;
    typedef const value_type &reference;
    typedef const value_type *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    /**
     * Constructor
     * @param hash_map
     * @param is_begin
     */
    ConstIterator (const HashMap<KeyT, ValueT>& hash_map, bool is_begin);

    /**
     * Forward Iterator (rhs)
     * @return
     */
    ConstIterator operator++ ()
    {
      if (bucket_ind >= hash_map.capacity ())
        {
          throw std::invalid_argument (INVALID_MSG);
        }
      if (pair_ind < hash_map.buckets[bucket_ind].size () - 1)
        {
          pair_ind++;
          return *this;
        }
      for (int i = bucket_ind + 1; i < hash_map.capacity (); i++)
        {
          if (!hash_map.buckets[i].empty ())
            {
              pair_ind = 0;
              bucket_ind = i;
              return *this;
            }
        }
      bucket_ind = hash_map.capacity ();
      pair_ind = 0;
      return *this;
    }

    /**
     * Forward iterator (lhs)
     * @return
     */
    ConstIterator operator++ (int)
    {
      ConstIterator tmp_it(*this);
      ++(*this);
      return tmp_it;
    }

    /**
     * Operator ==
     * @param rhs
     * @return true if the iterators point to same pair in same HashMap object
     */
    bool operator== (const ConstIterator &rhs) const
    { return (std::addressof(this->hash_map) == std::addressof(rhs.hash_map)
    && bucket_ind == rhs.bucket_ind && pair_ind == rhs.pair_ind); }

    bool operator== (ConstIterator &rhs) const
    { return (std::addressof(this->hash_map) == std::addressof(rhs.hash_map)
    && bucket_ind == rhs.bucket_ind && pair_ind == rhs.pair_ind); }

    bool operator!= (const ConstIterator &rhs) const
    { return !(*this == rhs); }

    bool operator!= (ConstIterator &rhs) const
    { return !(*this == rhs); }

    // Access and Assigment operators
    reference operator* ()
    { return hash_map.buckets[bucket_ind][pair_ind]; }

    reference operator* () const
    { return hash_map.buckets[bucket_ind][pair_ind]; }

    pointer operator-> ()
    { return &(operator* ()); }

    pointer operator-> () const
    { return &(operator* ()); }
  };
};

template<class KeyT, class ValueT>
int HashMap<KeyT,ValueT>:: hash_func (const KeyT &key) const
{
  int x = std::hash<KeyT>{}(key);
  return x & (_capacity - HASH_HELP);
}

template<class KeyT, class ValueT>
HashMap<KeyT,ValueT>:: HashMap (const std::vector<KeyT> &keyVec,
                                const std::vector<ValueT> &valVec): HashMap ()
{
  if (keyVec.size () != valVec.size ())
    {
      throw std::length_error (VECTOR_LENGTH);
    }
  for (int i = 0; i < (int) keyVec.size (); i++)
    {
      (*this)[keyVec[i]] = valVec[i];
    }
};

template<class KeyT, class ValueT>
HashMap<KeyT,ValueT>:: HashMap (const HashMap<KeyT, ValueT> &other)
{
  _size=INIT;
  _capacity = other._capacity;
  buckets = new bucket[_capacity];
  for (int i = 0; i < _capacity; i++)
    {
      for (int j = 0; j < (int) other.buckets[i].size (); j++)
        {
          buckets[i].push_back (other.buckets[i][j]);
          _size++;
        }
    }
};

template<class KeyT, class ValueT>
HashMap<KeyT,ValueT>& HashMap<KeyT,ValueT>:: operator=
    (const HashMap<KeyT, ValueT> &other)
{
  if (this == &other)
    {
      return *this;
    }
  clear ();
  delete[] buckets;
  _capacity = other._capacity;
  _size = INIT;
  buckets = new bucket[_capacity];
  for (int i = 0; i < _capacity; i++)
    {
      for (int j = 0; j < other.buckets[i].size (); j++)
        {
          buckets[i].push_back (other.buckets[i][j]);
          _size++;
        }
    }
  return *this;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT,ValueT>:: contains_key (const KeyT &key) const
{
  int key_ind = hash_func (key);
  for (int i=0;i< (int) buckets[key_ind].size();i++)
    {
      if (buckets[key_ind][i].first == key)
        {
          return true;
        }
    }
  return false;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT,ValueT>:: insert (const KeyT &key, const ValueT &value)
{
  if (contains_key (key))
    {
      return false;
    }
  int bucket_index = hash_func (key);
  auto new_pair = std::pair<KeyT, ValueT> (key, value);
  buckets[bucket_index].push_back (new_pair);
  _size++;
  update_capacity (true);
  return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT,ValueT>:: empty () const
{
  return (_size==INIT);
}

template<class KeyT, class ValueT>
void HashMap<KeyT,ValueT>:: clear ()
{
  if (empty ())
    {
      return;
    }
  for (int i = 0; i < _capacity; i++)
    {
      buckets[i].clear();
    }
  _size = INIT;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT,ValueT>:: erase (const KeyT &key)
{
  int key_ind = hash_func (key);
  auto pair_it = buckets[key_ind].begin ();
  while (pair_it != buckets[key_ind].end())
    {
      if (pair_it->first == key)
        {
          buckets[key_ind].erase (pair_it);
          --_size;
          update_capacity (false);
          return true;
        }
      pair_it++;
    }
  return false;
}

template<class KeyT, class ValueT>
int HashMap<KeyT,ValueT>:: bucket_size (const KeyT &key)
{
  if (!contains_key (key))
    {
      throw std::invalid_argument(INVALID_MSG);
    }
  int key_ind = hash_func (key);
  return buckets[key_ind].size ();
}

template<class KeyT, class ValueT>
int HashMap<KeyT,ValueT>:: bucket_index (const KeyT &key) const
{
  if (!contains_key (key))
    {
      throw std::invalid_argument(INVALID_MSG);
    }
  return hash_func (key);
}

template<class KeyT, class ValueT>
ValueT& HashMap<KeyT,ValueT>:: at (const KeyT &key)
{
  int ind = hash_func (key);
  int length = buckets[ind].size ();
  for (int i = 0; i < length; i++)
    {
      if (buckets[ind][i].first == key)
        {
          return buckets[ind][i].second;
        }
    }
  throw std::invalid_argument (KEY_NOT_FOUND);
}

template<class KeyT, class ValueT>
const ValueT & HashMap<KeyT,ValueT>:: at (const KeyT &key) const
{
  int ind = hash_func (key);
  for (int i = 0; i < (int) buckets[ind].size (); i++)
    {
      if (buckets[ind][i].first == key)
        {
          return buckets[ind][i].second;
        }
    }
  throw std::invalid_argument (INVALID_MSG);
}

template<class KeyT, class ValueT>
ValueT & HashMap<KeyT,ValueT>:: operator[] (const KeyT &key)
{
  try
    {
      ValueT& val = at (key);
      return val;
    }
  catch (const std::invalid_argument &e)
    {
      buckets[hash_func (key)].push_back
      (std::pair<KeyT, ValueT> (key, ValueT ()));
      _size++;
      update_capacity (true);
      return at (key);
    }
}

template<class KeyT, class ValueT>
const ValueT & HashMap<KeyT,ValueT>:: operator[] (const KeyT &key) const
{
  return at (key);
}

template<class KeyT, class ValueT>
bool HashMap<KeyT,ValueT>:: operator== (const HashMap<KeyT,ValueT> &other)
    const
{
  if (_size != other._size)
    {
      return false;
    }
  for (int i=0;i< (int) other._capacity;i++){
      for (auto& it : other.buckets[i]){
          if (!contains_key (it.first))
            {
              return false;
            }
          if (it.second != at (it.first))
            {
              return false;
            }
        }
    }
  return true;
}

template<class KeyT, class ValueT>
bool HashMap<KeyT,ValueT>:: operator!= (const HashMap &other) const
{
  return !(operator== (other));
}

template<class KeyT, class ValueT>
void HashMap<KeyT,ValueT>:: rehash (double mult)
{
  if (mult == 0){
      delete[] buckets;
      auto *new_buckets = new bucket[ERASE_LAST_INIT];
      buckets=new_buckets;
      _capacity=ERASE_LAST_INIT;
  }
  else{
      auto *new_buckets = new bucket[(int)(_capacity * mult)];
      for (int i=0; i<capacity();i++){
          for (const auto& it : buckets[i]){
              new_buckets[hash_func (it.first)].push_back(it);
            }
        }
      delete[] buckets;
      _capacity *= mult;
      buckets = new_buckets;
  }
}

template<class KeyT, class ValueT>
void HashMap<KeyT,ValueT>:: update_capacity (bool is_grow)
{
  double mult;
  if (is_grow)
    {
      if (get_load_factor () > LOAD_FACTOR_MAX){
          mult = MULT;
      }
      else{
          return;
      }
    }
  else
    {
      if (_size == INIT)
        {
          mult = ERASE_LAST_INDICATION;
        }
      else if (get_load_factor () < LOAD_FACTOR_MIN)
        {
          mult = DIV;
        }
      else{
          return;
      }
    }
  rehash (mult);
}


template<class KeyT, class ValueT>
HashMap<KeyT,ValueT>:: ConstIterator:: ConstIterator
(const HashMap<KeyT, ValueT>& hash_map, bool is_begin)
: hash_map (hash_map)
    {
        if (!is_begin)
        {
          bucket_ind = hash_map.capacity ();
          pair_ind = INIT;
          return;
        }
        for (int i = 0; i < hash_map.capacity (); i++)
        {
          if (!hash_map.buckets[i].empty ())
            {
              bucket_ind = i;
              pair_ind = INIT;
              return;
            }
        }
        bucket_ind = hash_map.capacity ();
        pair_ind = INIT;
    }

#endif
