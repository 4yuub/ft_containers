#include <iostream>
#include <string>

#if defined(USING_STD)
# define NS std
#include <map>
#elif defined(USING_FT)
# define NS ft
#include "map.hpp"
#endif

#ifdef NS

int map_test(void) {
  std::cout << "map test:\n";
  NS::map<std::string, int> mp;
  NS::map<int, std::string> mp2;
  mp["hello"] = 3;
  mp["world"] = 5;
  for (int i = 0; i < 1e4; ++i) {
    mp2[i] += 'a';
  }

  NS::pair<NS::map<int, std::string>::iterator, NS::map<int, std::string>::iterator> bounds;
  bounds = mp2.equal_range(400);
  std::cout << "Lower bound: " << bounds.first->first << " " << bounds.first->second << std::endl;
  std::cout << "Upper bound: " << bounds.second->first << " " << bounds.second->second << std::endl;
  return 0;
}

#endif