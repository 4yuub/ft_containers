#include <iostream>
#include "iterator.hpp"

#if defined(USING_STD)
# define NS std
#elif defined(USING_FT)
# define NS ft
#endif

#ifdef NS


class MyIterator : public NS::iterator<std::input_iterator_tag, int>
{
  int* p;
public:
  typedef ptrdiff_t differece_t;
  MyIterator() {}
  MyIterator(int* x) :p(x) {}
  MyIterator(const MyIterator& mit) : p(mit.p) {}
  MyIterator& operator++() {++p;return *this;}
  MyIterator& operator--() {--p;return *this;}
  MyIterator operator++(int) {MyIterator tmp(*this); operator++(); return tmp;}
  MyIterator operator--(int) {MyIterator tmp(*this); operator--(); return tmp;}
  bool operator==(const MyIterator& rhs) const {return p==rhs.p;}
  bool operator!=(const MyIterator& rhs) const {return p!=rhs.p;}
  int& operator*() {return *p;}
};

int test_iterator(void)
{
  std::cout << "iterator test: \n";
  int numbers[]={10,20,30,40,50};
  MyIterator from(numbers);
  MyIterator until(numbers+5);
  for (MyIterator it=from; it!=until; it++)
    std::cout << *it << ' ';
  std::cout << '\n';

  std::cout << "reverse iterator test: \n";
  NS::reverse_iterator<MyIterator> rfrom(until);
  NS::reverse_iterator<MyIterator> runtil(from);
  for (NS::reverse_iterator<MyIterator> it=rfrom; it.base()!=runtil.base(); it++)
    std::cout << *it << ' ';
  std::cout << '\n';
  return 0;
}

#endif