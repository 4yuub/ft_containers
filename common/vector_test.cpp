#include <iostream>

#include <iostream>

#if defined(USING_STD)
# define NS std
#include <vector>
#elif defined(USING_FT)
# define NS ft
#include "vector"
#endif

#ifdef NS

int vector_test(void) {
    std::cout << "vector test: \n";
    int numbers[]={10,20,30,40,50};
    NS::vector<int> v(numbers, numbers+5);
    // push_back && insert
    v.push_back(60);
    v.insert(v.begin(), 5);
    // erase
    v.erase(v.begin());
    v.erase(v.begin()+2, v.end()-1);
    // resize
    v.resize(8, 100);
    v.resize(12);
    // swap
    NS::vector<int> v2(v);
    v.swap(v2);
    // clear
    v.clear();
    // assign
    v.assign(v2.begin(), v2.end());
    // pop_back
    v.pop_back();
    // at
    std::cout << v.at(0) << std::endl;
    // front
    std::cout << v.front() << std::endl;
    // back
    std::cout << v.back() << std::endl;
    // begin
    std::cout << *v.begin() << std::endl;
    // end
    std::cout << *(v.end()-1) << std::endl;
    // rbegin
    std::cout << *v.rbegin() << std::endl;
    // rend
    std::cout << *(v.rend()-1) << std::endl;
    // empty
    std::cout << v.empty() << std::endl;
    // size
    std::cout << v.size() << std::endl;
    // max_size
    std::cout << v.max_size() << std::endl;
    // capacity
    std::cout << v.capacity() << std::endl;
    // reserve
    v.reserve(20);
    std::cout << v.capacity() << std::endl;
    for (NS::vector<int>::iterator it=v.begin(); it!=v.end(); it++)
        std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}

#endif