#ifndef FIBONACCI_NUMBERS_H_
#define FIBONACCI_NUMBERS_H_

#include <type_traits>


template<std::size_t N>
struct FibonacciNumber :
        std::integral_constant<std::size_t, FibonacciNumber<N - 1>::value + FibonacciNumber<N - 2>::value>
{
    static_assert(FibonacciNumber<N - 1>::value + FibonacciNumber<N - 2>::value >= FibonacciNumber<N - 1>::value,
                  "Overflow occurred");
};

template<>
struct FibonacciNumber<0> :
        std::integral_constant<std::size_t, 1>
{};

template<>
struct FibonacciNumber<1> :
        std::integral_constant<std::size_t, 1>
{};

#endif
