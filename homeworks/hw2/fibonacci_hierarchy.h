#ifndef FIBONACCI_HIERARCHY_H_
#define FIBONACCI_HIERARCHY_H_

#include "linear_hierarchy.h"
#include "fibonacci_numbers.h"


template<typename TL, template<typename T, class BaseClass> class Unit, size_t BranchNum>
struct GenFibonacciHierarchyHelper;

template<typename TL, template<typename T, class BaseClass> class Unit, size_t BranchNum>
struct GenFibonacciHierarchyHelper
        : GenLinearHierarchy<typename Sublist<0, FibonacciNumber<BranchNum>::value + 1, TL>::type_list, Unit>,
          GenFibonacciHierarchyHelper<typename Sublist<
                  FibonacciNumber<BranchNum>::value + 1, Length<TL>::value, TL>::type_list, Unit, BranchNum + 1> {
};

template<template<typename T, class BaseClass> class Unit, size_t BranchNum>
struct GenFibonacciHierarchyHelper<EmptyList, Unit, BranchNum> {
};


// GenLinearHierarchy
// generating fibonacci hierarchy (see task description) from type list TL
template<typename TL, template<typename T, class BaseClass> class Unit>
struct GenFibonacciHierarchy
        : GenFibonacciHierarchyHelper<TL, Unit, 0> {
};


#endif