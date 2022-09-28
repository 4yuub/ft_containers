#include <iostream>
#include "common.hpp"
#include "tests.hpp"

#if defined(USING_STD)
# define NS std
#elif defined(USING_FT)
# define NS ft
#endif

int main(void) {
#ifndef NS
    std::cerr << "Error: undefined namespace" << std::endl;
    return 1;
#else
    test_iterator();
    test_type_traits();
    return 0;
#endif
}