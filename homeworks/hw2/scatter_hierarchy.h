#ifndef SCATTER_HIERARCHY_H_
#define SCATTER_HIERARCHY_H_

#include "linear_hierarchy.h"

/*
 * Generating Scatter hierarchy (look at Alexandresku's book).
 * Slightly changed to C++ standard 11.
 */

template<typename T>
struct Holder : Unit<T, NullType>{};

// GenScatterHierarchy
template<class TL, template<class T> class Holder>
struct GenScatterHierarchy;

template<template<class T> class Holder>
struct GenScatterHierarchy<EmptyList, Holder> {
    using type_list = NullType;
    using left_base = NullType;
    using right_base = NullType;
};

template<typename Head, typename ...Tail, template<class T> class Holder>
struct GenScatterHierarchy<TypeList<Head, Tail...>, Holder> :
        Holder<Head>,
        GenScatterHierarchy<TypeList<Tail...>, Holder> {
    using type_list = TypeList<Head, Tail...>;
    using left_base = Holder<Head>;
    using right_base = GenScatterHierarchy<TypeList<Tail...>, Holder>;
};

#endif