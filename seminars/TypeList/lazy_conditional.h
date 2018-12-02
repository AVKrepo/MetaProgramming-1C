#ifndef LAZY_CONDITIONAL_H_
#define LAZY_CONDITIONAL_H_

#include <type_traits>

template<bool B, template<typename...> class TrueTemplate,
        template<typename...> class FalseTemplate, typename ArgsTuple>
struct LazyConditional;

template<template<typename...> class TrueTemplate,
        template<typename...> class FalseTemplate, typename ... Args>
struct LazyConditional<true, TrueTemplate, FalseTemplate, std::tuple<Args...>> {
    using type = TrueTemplate<Args...>;
};

template<template<typename...> class TrueTemplate,
        template<typename...> class FalseTemplate, typename ... Args>
struct LazyConditional<false, TrueTemplate, FalseTemplate, std::tuple<Args...>> {
    using type = FalseTemplate<Args...>;
};

#endif