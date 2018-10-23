#include <iostream>
#include <type_traits>

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
    static_assert(I < Length<TypeList<Args...>>::value, "Index out of bounds");
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
    using type_list = TypeList<T, Args...>;
};

template<typename T>
struct Insert<0, T, EmptyList> {
    using type_list = TypeList<T>;
};

template<size_t I, typename T, typename ...Args>
struct Insert<I, T, TypeList<Args...>> {
    static_assert(I <= Length<TypeList<Args...>>::value);
    using type_list = typename AddToBeginning<typename TypeList<Args...>::Head, typename Insert<
            I - 1, T, typename TypeList<Args...>::Tail>::type_list>::type_list;
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
    static_assert(Length<TypeList<Args...>>::value > 0 && I < Length<TypeList<Args...>>::value);
    using type_list = typename AddToBeginning<typename TypeList<Args...>::Head, typename RemoveFrom<
            I - 1, typename TypeList<Args...>::Tail>::type_list>::type_list;
};


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
    std::cout << "Hello, World!" << std::endl;
    return 0;
}