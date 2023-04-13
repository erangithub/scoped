/*
scoped.h

Provides a templated scoped class for "Scope-Sensitive Programming"

The scoped class provides a mechanism for controlling the behavior of code within a scope
without requiring explicit argument passing. The class is thread-safe and allows for creating
a chain of scopes that can be traversed in both directions.

Author: Eran Talmor 2023, the.eran.talmor@gmail.com
*/

#ifndef _INCLUDE_SCOPED_H_
#define _INCLUDE_SCOPED_H_

#include <utility>

// An abstract class template for managing resources within a specific scope.
template <class T, class ...Tags>
class abstract_scoped
{
public:
    // Constructor that adds the current instance to the top of the linked list of instances.
    explicit abstract_scoped() : m_next(s_top)
    {
        if (s_bottom == nullptr) s_bottom = this;
        s_top = this;
        if (m_next) m_next->m_prev = this;
    }

    // Destructor that removes the current instance from the top of linked list of instances.
    ~abstract_scoped()
    {
        if (m_next) m_next->m_prev = nullptr;
        s_top = m_next;
        if (s_bottom == this) s_bottom = nullptr;
    }

    // Returns a reference to the value being scoped.
    virtual T& value() = 0;

    // Returns a pointer to the next instance of the scoped class in the linked list of instances.
    abstract_scoped* next()
    {
        return m_next;
    }

    // Returns a pointer to the previous instance of the scoped class in the linked list of instances.
    abstract_scoped* prev()
    {
        return m_prev;
    }

    // Returns a pointer to the top instance of the scoped class in the linked list of instances.
    static abstract_scoped* top()
    {
        return s_top;
    }

    // Returns a pointer to the bottom instance of the scoped class in the linked list of instances.
    static abstract_scoped* bottom()
    {
        return s_bottom;
    }

    // Delete the copy constructor, copy assignment operator, move constructor, and move assignment operator
    // to ensure that scoped objects are not copied or moved to a container that may not preserve the stack order.
    // This is necessary because the prev and next pointers of the scoped objects will not necessarily point to
    // objects that are adjacent in the stack order if the objects are copied or moved to a container.
    abstract_scoped(const abstract_scoped&) = delete;
    abstract_scoped& operator=(const abstract_scoped&) = delete;
    abstract_scoped(abstract_scoped&&) = delete;
    abstract_scoped& operator=(abstract_scoped&&) = delete;

    // Disable the use of the default new and delete operators, as scoped instances should not be created on the heap.
    static void* operator new(size_t) = delete;          // standard new
    static void* operator new[](size_t) = delete;        // array new
    static void* operator new(size_t, void*) = delete;   // placement new
    static void* operator new[](size_t, void*) = delete; // placement array new
    
    static void operator delete(void *) = delete;
    static void operator delete[](void *) = delete;

private:
    // Pointers to the next and previous instances of the scoped class in the linked list of instances.
    abstract_scoped* m_next;
    abstract_scoped* m_prev;

    // Thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
    static thread_local abstract_scoped* s_top;
    static thread_local abstract_scoped* s_bottom;
};

// Define the thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
template<class T, class ...Tags>
thread_local abstract_scoped<T, Tags...>* abstract_scoped<T, Tags...>::s_top;

template<class T, class ...Tags>
thread_local abstract_scoped<T, Tags...>* abstract_scoped<T, Tags...>::s_bottom;

// A class template for scoping values of type T, while interfacing them with the abstract scope for T's base class B.
template<class T, class B, class ...Tags> class polymorphic_scoped : public abstract_scoped<B, Tags...>
{
public:
    using Base = abstract_scoped<B, Tags...>;

    // Constructor that initializes the value being scoped with any number of arguments.
    template <class... Args>
    explicit polymorphic_scoped(Args&&... args) : Base(), m_value{std::forward<Args>(args)...}
    {}
    
    B& value() override { return m_value; }
private:
    T m_value;
};

template<class T, class ...Tags> using scoped = polymorphic_scoped<T, T, Tags...>;

#endif // _INCLUDE_SCOPED_H_
