#include <iostream>
#include <string>

#if defined(USING_STD)
# define NS std
#include <set>
#elif defined(USING_FT)
# define NS ft
#include "set.hpp"
#endif

#ifdef NS

int set_test ()
{
  std::cout << "set test:\n";
  NS::set<int> st;

  for (int i = 0; i < 1e3; ++i) st.insert(i);
  for (int i = 0; i < 1e3; ++i) st.insert(i);
  for (int i = 0; i < 1e3; ++i) st.insert(i);
  NS::set<int>::iterator it;
  for (it = st.begin(); it != st.end(); it++) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;
  return 0;
}

#endif