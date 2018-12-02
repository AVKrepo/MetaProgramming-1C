#include <iostream>
#include "scatter_hierarchy.h"
#include "fibonacci_hierarchy.h"

int main() {
    std::cout << FibonacciNumber<92>::value << std::endl;
    using list = TypeList<int, char, float, double, float, std::string>;
    GenLinearHierarchy<list, Unit> linear_hierarchy;
//    GenScatterHierarchy<list, Holder> scatter_hierarchy;
    GenFibonacciHierarchy<list, Unit> fibonacci_hierarchy;
    return 0;
}