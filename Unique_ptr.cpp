#include <iostream>
#include <utility>
#include <cassert>
#include <stdexcept>

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
            reset(other.release());
        }
        return *this;
    }

    T& operator*() const {
        if (ptr_ == nullptr) {
            throw std::runtime_error("SimpleUniquePtr: dereferencing null pointer");
        }
        return *ptr_;
    }

    T* operator->() const {
        if (ptr_ == nullptr) {
            throw std::runtime_error("SimpleUniquePtr: arrow operator on null pointer");
        }
        return ptr_;
    }

    T* get() const noexcept { return ptr_; }
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

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
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
    try {
        SimpleUniquePtr<Foo> p(new Foo());
        (*p).hello();
        p->hello();

        SimpleUniquePtr<Foo> empty;
        try {
            (*empty).hello();
        }
        catch (const std::runtime_error& e) {
            std::cout << "Exception caught: " << e.what() << '\n';
        }

        Foo* raw = p.release();
        delete raw;

        SimpleUniquePtr<Foo> p2(new Foo());
        SimpleUniquePtr<Foo> p3 = std::move(p2);
        assert(p2.get() == nullptr);

        std::cout << "All tests passed!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}