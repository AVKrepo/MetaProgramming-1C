#ifndef LINEAR_HIERARCHY_H_
#define LINEAR_HIERARCHY_H_

#include "../../seminars/TypeList/type_list.h"

// Unit
// Contains type T, derived from BaseClass
template<typename T, class BaseClass>
struct Unit;

template<typename T, class BaseClass>
struct Unit : BaseClass {
    T value;
};


// GenLinearHierarchy
// generating linear hierarchy from type list TL
template<typename TL, template<typename T, class BaseClass> class Unit>
struct GenLinearHierarchy;

template<typename Head, typename ...Tail, template<typename T, class BaseClass> class Unit>
struct GenLinearHierarchy<TypeList<Head, Tail...>, Unit>
        : Unit<Head, GenLinearHierarchy<TypeList<Tail...>, Unit>> {
};

template<template<typename T, class BaseClass> class Unit>
struct GenLinearHierarchy<EmptyList, Unit> : Unit<NullType, NullType> {
};


#endif