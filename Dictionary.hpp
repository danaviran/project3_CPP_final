#ifndef DICTIONARY_EX6
#define DICTIONARY_EX6

#include "HashMap.hpp"
#define INVALID_MSG "Error: Invalid argument"
using std::string;

/**
 * InvalidKey exception raising class
 * Inherits from std::invalid_argument class
 */
class InvalidKey : public std::invalid_argument {
 public:
  // Constructors
  explicit InvalidKey () : std::invalid_argument (INVALID_MSG)
  {};
  explicit InvalidKey (const string &msg) : std::invalid_argument (msg)
  {};
};

/**
 * Dictionary class
 * Represents a inherited class of HashMap that maps string keys to string vals
 */
class Dictionary : public HashMap<string, string> {
 public:
  // Constructors
  Dictionary (const std::vector<string> &key_vec, const std::vector<string>
      &value_vec)
      : HashMap<string, string> (key_vec, value_vec)
  {};

  /**
   * A template iterator function to go throw unknown generic container
   * that contain pairs of string keys and values
   * @tparam T Class of Container
   * @param begin Iterator to beginning of container (first pair)
   * @param end Iterator to end of container (nullptr)
   */
  template<typename T>
  void update ( T begin, T end){
    for (;begin!=end;begin++)
      {
        HashMap<string, string>::operator[] (begin->first) = begin->second;
      }
  }

  /**
   * Override function of virtual erase function in HashMap class
   * differs from it by raising InvalidKey exception if key not found
   * @param key
   * @return true upon success
   */
  bool erase (const string &key) override{
    if (!HashMap<string, string>::erase (key))
      {
        auto error = InvalidKey ();
      }
    return true;
  }
};

#endif //DICTIONARY_EX6