#ifndef UNIQUE_PTR_H_
#define UNIQUE_PTR_H_

template<typename T>
class UniquePtr {
public:
    UniquePtr(T *ptr = nullptr)
            : ptr_(ptr) {}

    void reset(T *ptr = nullptr) {
        T *old_ptr = ptr_;
        ptr_ = ptr;
        if (old_ptr != nullptr) {
            delete old_ptr;
        }
    }

    ~UniquePtr() {
        reset();
    }

    UniquePtr(const UniquePtr &anotherPtr) = delete;

    UniquePtr &operator=(UniquePtr &anotherPtr) = delete;

    UniquePtr(UniquePtr<T> &&anotherPtr) {
        ptr_ = anotherPtr.ptr_;
        anotherPtr.ptr_ = nullptr;
    }

    UniquePtr &operator=(UniquePtr<T> anotherPtr) {
        if (&anotherPtr == this) {
            return *this;
        }
        reset(anotherPtr.ptr_);
        anotherPtr.ptr_ = nullptr;
        return *this;
    }

    T *operator->() {
        return ptr_;
    }

    bool isNullptr() {
        return ptr_ == nullptr;
    }

private:
    T *ptr_;
};

#endif