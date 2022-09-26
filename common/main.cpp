#include <iostream>
#include "common.hpp"

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
    // testing goes here
    return 0;
#endif
}