#include <iostream>

#if defined(USING_STD)
# define NS std
#include <stack>
#elif defined(USING_FT)
# define NS ft
#include "stack.hpp"
#endif

#ifdef NS

int stack_test(void) {
    NS::stack<int> mystack;
    
    std::cout << "stack test: " << std::endl;
    for (int i = 0; i < 5; i++)
        mystack.push(i);
    // stack size
    std::cout << "size: " << mystack.size() << std::endl;
    std::cout << "Popping out elements..." << std::endl;
    while (!mystack.empty())
    {
        std::cout << ' ' << mystack.top();
        mystack.pop();
    }
    std::cout << std::endl;
    return 0;
}

#endif