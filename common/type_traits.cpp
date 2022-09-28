#include <iostream>

#if defined(USING_STD)
# define NS std
#include <type_traits>
#elif defined(USING_FT)
# define NS ft
#include "type_traits.hpp"
#endif
#define SHOW(...) std::cout << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'
 
#ifdef NS

template <typename T>
typename NS::enable_if<NS::is_integral<T>::value, bool>::type are_integrals(T lhs, T rhs) {
  (void) lhs, (void) rhs;
  return true;
}

template <typename T>
typename NS::enable_if<!NS::is_integral<T>::value, bool>::type are_integrals(T lhs, T rhs) {
  (void) lhs, (void) rhs;
  return false;
}

int test_type_traits(void)
{
  std::cout << "type traits test:\n";
  std::cout << std::boolalpha;
  std::cout << "is_integral:" << std::endl;
  std::cout << "char: " << NS::is_integral<char>::value << std::endl;
  std::cout << "int: " << NS::is_integral<int>::value << std::endl;
  std::cout << "float: " << NS::is_integral<float>::value << std::endl;
  std::cout << "enable if test:\n";
  SHOW( are_integrals(1, 2) );
  SHOW( are_integrals(1.2f, 2.0f) );
  return 0;
}

#endif