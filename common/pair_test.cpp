#include <iostream>
#include <string>

#if defined(USING_STD)
# define NS std
#include <utility>
#elif defined(USING_FT)
# define NS ft
#include "utility.hpp"
#endif

#ifdef NS

int pair_test(void) {
  NS::pair <std::string,int> planet, homeplanet;
  planet = NS::make_pair("Earth", 6371);

  homeplanet = planet;

  NS::pair <std::string, int> cpy(homeplanet);
  std::cout << "Home planet: " << homeplanet.first << '\n';
  std::cout << "Planet size: " << cpy.second << '\n';
  return 0;
}

#endif