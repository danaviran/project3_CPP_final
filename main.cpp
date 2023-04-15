#include <iostream>
#include "HashMap.hpp"
#include <utility>
#include "sstream"
#include <string>
using std::string;

#define assert(condition) if(!(condition)) throw runtime_error(format_what(#condition, \
__LINE__))

string format_what (const string &condition, int line)
{
  std::ostringstream stream;
  stream << "assert(" << condition << ")" << ", line: " << line;
  return stream.str ();
}

int main ()
{
  auto h = HashMap<int, int> ();
  std::cout << (h.size() == 0) << std::endl;
  std::cout << (h.capacity() == 16) << std::endl;
  h.insert (1,10);
  std::cout << (h.size() == 1) << std::endl;
  std::cout << (h.capacity() == 16) << std::endl;
  std::cout << "DONE\n";
  h[1] = 8;
  auto dana = HashMap<int, int> ({1,1,1},{2,2,3});
  std::cout << dana.size();
  std::cout << dana[1];
}