#include <iostream>
#include <assert.h>
#include "unique_ptr.h"

class SomeClass {
public:
    SomeClass(int value)
            : data(value) {}

    int getData() {
        return data;
    }

private:
    int data;
};

int main() {
    SomeClass *classPtr = new SomeClass(5);
    UniquePtr<SomeClass> ptr1 = UniquePtr<SomeClass>(classPtr);
    std::cout << ptr1->getData() << std::endl;
    /// Constructors
//    UniquePtr<SomeClass> ptr2 = UniquePtr<SomeClass>(ptr1); // error!
    UniquePtr<SomeClass> ptr2 = UniquePtr<SomeClass>(std::move(ptr1));
    assert(ptr1.isNullptr() && !ptr2.isNullptr());
    std::cout << ptr2->getData() << std::endl;
    UniquePtr<SomeClass> ptr3 = UniquePtr<SomeClass>();
    assert(ptr3.isNullptr() && !ptr2.isNullptr());
//    ptr3 = ptr2; // error!
    ptr3 = std::move(ptr2);
    assert(!ptr3.isNullptr() && ptr2.isNullptr());
    std::cout << ptr3->getData() << std::endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}