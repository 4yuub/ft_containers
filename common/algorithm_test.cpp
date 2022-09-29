#include <iostream>
#include <cctype>
#include <vector>

#if defined(USING_STD)
# define NS std
#include <algorithm>
#elif defined(USING_FT)
# define NS ft
#include "algorithm.hpp"
#endif

#ifdef NS

// lixicographical_compare
bool mycomp (char c1, char c2)
{ return std::tolower(c1)<std::tolower(c2); }

int lexicographical_compare_test(void) {
  char foo[]="Apple";
  char bar[]="apartment";

  std::cout << std::boolalpha;

  std::cout << "Comparing foo and bar lexicographically (foo<bar):\n";

  std::cout << "Using default comparison (operator<): ";
  std::cout << NS::lexicographical_compare(foo,foo+5,bar,bar+9);
  std::cout << '\n';

  std::cout << "Using mycomp as comparison object: ";
  std::cout << NS::lexicographical_compare(foo,foo+5,bar,bar+9,mycomp);
  std::cout << '\n';

  return 0;
}

// equl testing
bool mypredicate (int i, int j) {
  return (i==j);
}

int equal_test (void) {
  int myints[] = {20,40,60,80,100};               //   myints: 20 40 60 80 100
  std::vector<int>myvector (myints,myints+5);     // myvector: 20 40 60 80 100

  // using default comparison:
  if ( NS::equal (myvector.begin(), myvector.end(), myints) )
    std::cout << "The contents of both sequences are equal.\n";
  else
    std::cout << "The contents of both sequences differ.\n";

  myvector[3]=81;                                 // myvector: 20 40 60 81 100

  // using predicate comparison:
  if ( NS::equal (myvector.begin(), myvector.end(), myints, mypredicate) )
    std::cout << "The contents of both sequences are equal.\n";
  else
    std::cout << "The contents of both sequences differ.\n";

  return 0;
}

#endif