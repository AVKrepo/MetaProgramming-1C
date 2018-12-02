#ifndef TYPE_LIST_H_
#define TYPE_LIST_H_

#include <type_traits>
#include <iostream>

/// TypeList
struct NullType {
};

template<typename... Args>
struct TypeList {
    using Head = NullType;
    using Tail = NullType;
};

typedef TypeList<> EmptyList;

template<typename H, typename... T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
};

/// Length
template<typename TL>
struct Length :
        std::integral_constant<size_t, 0> {
};

template<>
struct Length<EmptyList> :
        std::integral_constant<size_t, 0> {
};

template<typename ...Args>
struct Length<TypeList<Args...>> :
        std::integral_constant<size_t, sizeof ...(Args)> {
};

/// TypeAt
template<size_t I, typename TL>
struct TypeAt {
    using type = NullType;
};

template<typename ...Args>
struct TypeAt<0, TypeList<Args...>> {
    using type = typename TypeList<Args...>::Head;
};

template<size_t I, typename ...Args>
struct TypeAt<I, TypeList<Args...>> {
    static_assert(I < Length<TypeList<Args...>>::value, "TypeAt::Index out of bounds");
    using type = typename TypeAt<I - 1, typename TypeList<Args...>::Tail>::type;
};

/// operator<<
std::ostream &operator<<(std::ostream &os, EmptyList) {
    os << "{}";
    return os;
}

template<typename TL>
void PrintTypeListHelper(TL, std::ostream &os) {
}

template<typename T>
void PrintTypeListHead(T, std::ostream &os) {
    os << typeid(T).name();
}

template<typename ...Args>
void PrintTypeListHead(TypeList<Args...> tl, std::ostream &os) {
    os << tl;
}

template<typename Head, typename ...Args>
void PrintTypeListHelper(TypeList<Head, Args...>, std::ostream &os) {
    PrintTypeListHead(Head(), os);
    if (Length<TypeList<Args...>>::value > 0) {
        os << ' ';
        PrintTypeListHelper<Args...>(TypeList<Args...>(), os);
    }
}

template<typename ...Args>
std::ostream &operator<<(std::ostream &os, TypeList<Args...> tl) {
    os << '{';
    PrintTypeListHelper(tl, os);
    os << '}';
    return os;
}

/// AddToBeginning
template<typename H, typename TL>
struct AddToBeginning {
    using type_list = NullType;
};

template<typename H, typename ...Args>
struct AddToBeginning<H, TypeList<Args...>> {
    using type_list = TypeList<H, Args...>;
};


/// Insert
template<size_t I, typename T, typename TL>
struct Insert {
    using type_list = NullType;
};

template<typename T, typename ...Args>
struct Insert<0, T, TypeList<Args...>> {
    using type_list = typename AddToBeginning<T, TypeList<Args...>>::type_list;
};

template<typename T>
struct Insert<0, T, EmptyList> {
    using type_list = TypeList<T>;
};

template<size_t I, typename T, typename ...Args>
struct Insert<I, T, TypeList<Args...>> {
    static_assert(I <= Length<TypeList<Args...>>::value, "Insertion::Failed because of len(list) < I");
    using type_list = typename AddToBeginning<typename TypeList<Args...>::Head, typename Insert<
            I - 1, T, typename TypeList<Args...>::Tail>::type_list>::type_list;
};


/// PushBack
template<typename T, typename TL>
struct PushBack {
    using type_list = NullType;
};

template<typename T, typename ...Args>
struct PushBack<T, TypeList<Args...>> {
    using type_list = typename Insert<Length<TypeList<Args...>>::value, T, TypeList<Args...>>::type_list;
};

/// RemoveFrom
template<size_t I, typename TL>
struct RemoveFrom {
    using type_list = NullType;
};

template<typename ...Args>
struct RemoveFrom<0, TypeList<Args...>> {
    using type_list = typename TypeList<Args...>::Tail;
};

template<size_t I, typename ...Args>
struct RemoveFrom<I, TypeList<Args...>> {
    static_assert(Length<TypeList<Args...>>::value > 0, "RemoveFrom::Removing from empty list");
    static_assert(I < Length<TypeList<Args...>>::value, "RemoveFrom::Failed because of len(list) < I");
    using type_list = typename AddToBeginning<typename TypeList<Args...>::Head, typename RemoveFrom<
            I - 1, typename TypeList<Args...>::Tail>::type_list>::type_list;
};

/// Replace
template<size_t I, typename T, typename TL>
struct Replace {
    using type_list = NullType;
};

template<size_t I, typename T, typename... Args>
struct Replace<I, T, TypeList<Args...>> {
    using type_list = typename Insert<I, T, typename RemoveFrom<I, TypeList<Args...>>::type_list>::type_list;
};

/// Sublist
template<size_t FromIdx, size_t ToIdx, typename TL>
struct Sublist {
};

template<size_t FromIdx, size_t ToIdx>
struct Sublist<FromIdx, ToIdx, EmptyList> {
    using type_list = EmptyList;
};

template<size_t FromIdx, typename ...Args>
struct Sublist<FromIdx, FromIdx, TypeList<Args...>> {
    using type_list = EmptyList;
};

template<size_t FromIdx, size_t ToIdx, typename ...Args>
struct Sublist<FromIdx, ToIdx, TypeList<Args...>> {
//    static_assert(FromIdx <= ToIdx, "Invalid range");
    constexpr static size_t End = std::min(Length<TypeList<Args...>>::value, ToIdx);
    constexpr static size_t Begin = FromIdx % Length<TypeList<Args...>>::value;
    constexpr static bool is_valid = (FromIdx < End);
    using type_list = typename std::conditional<is_valid,
            typename PushBack<typename TypeAt<End - 1, TypeList<Args...>>::type, typename Sublist<Begin,
                    End - 1, TypeList<Args...>>::type_list>::type_list,
            EmptyList
    >::type;
};

#endif