#include <iostream>
#include <utility> 

template <typename T>
class SimpleUniquePtr {
public:
    explicit SimpleUniquePtr(T* ptr = nullptr) noexcept
        : ptr_(ptr) {
    }

    SimpleUniquePtr(const SimpleUniquePtr&) = delete;
    SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;

    SimpleUniquePtr(SimpleUniquePtr&& other) noexcept
        : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept {
        if (this != &other) {
            reset();           
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const noexcept {
        return *ptr_;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void reset(T* ptr = nullptr) noexcept {
        if (ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }

    ~SimpleUniquePtr() {
        delete ptr_;
    }

private:
    T* ptr_;
};


struct Foo {
    Foo() { std::cout << "Foo()\n"; }
    ~Foo() { std::cout << "~Foo()\n"; }
    void hello() { std::cout << "hello\n"; }
};

int main() {
    SimpleUniquePtr<Foo> p(new Foo);
    (*p).hello();
    p->hello();

    Foo* raw = p.release();
    delete raw;

    return 0;
}