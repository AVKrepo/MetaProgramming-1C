#include <iostream>
#include "type_list.h"


int main() {
    using list = TypeList<int, double, char>;
    using empty_list = TypeList<>;
    std::cout << Length<list>::value << " " << Length<empty_list>::value << std::endl;
    std::cout << std::is_same<TypeAt<2, list>::type, char>::value << " " <<
              std::is_same<TypeAt<1, list>::type, int>::value << std::endl;
//    std::cout << std::is_same<TypeAt<1, empty_list>::type, int>::value << std::endl; // static assert failed
    std::cout << list() << std::endl;
    std::cout << AddToBeginning<void *, list>::type_list() << std::endl;
//    std::cout << list() << std::endl; // list hasn't changed
    std::cout << Insert<3, float, list>::type_list() << std::endl;
    std::cout << RemoveFrom<0, list>::type_list() << RemoveFrom<1, list>::type_list()
              << RemoveFrom<2, list>::type_list() << std::endl;
    std::cout << Replace<1, float, list>::type_list() << std::endl;
    std::cout << list() << std::endl;
    std::cout << PushBack<int, list>::type_list() << std::endl;
    std::cout << empty_list() << std::endl;
    std::cout << PushBack<int, empty_list >::type_list() << std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}