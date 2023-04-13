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

// A class template for managing resources within a specific scope.
template <class T, class ...Tags>
class scoped
{
public:
    // Constructor that initializes the value being scoped with any number of arguments.
    template <class... Args>
    explicit scoped(Args&&... args) :
        m_value{std::forward<Args>(args)...},
        m_next(s_top)
    {
        // Add the current instance to the top of the linked list of instances.
        if (s_bottom == nullptr) s_bottom = this;
        s_top = this;
        if (m_next) m_next->m_prev = this;
    }

    // Destructor that removes the current instance from the top of linked list of instances.
    ~scoped()
    {
        if (m_next) m_next->m_prev = nullptr;
        s_top = m_next;
        if (s_bottom == this) s_bottom = nullptr;
    }

    // Returns a reference to the value being scoped.
    T& value()
    {
        return m_value;
    }

    // Returns a pointer to the next instance of the scoped class in the linked list of instances.
    scoped* next()
    {
        return m_next;
    }

    // Returns a pointer to the previous instance of the scoped class in the linked list of instances.
    scoped* prev()
    {
        return m_prev;
    }

    // Returns a pointer to the top instance of the scoped class in the linked list of instances.
    static scoped* top()
    {
        return s_top;
    }

    // Returns a pointer to the bottom instance of the scoped class in the linked list of instances.
    static scoped* bottom()
    {
        return s_bottom;
    }

private:
    // Disable the use of the default new and delete operators, as scoped instances should not be created on the heap.
    static void* operator new(size_t) = delete;          // standard new
    static void* operator new[](size_t) = delete;        // array new
    static void* operator new(size_t, void*) = delete;   // placement new
    static void* operator new[](size_t, void*) = delete; // placement array new
    
    static void operator delete(void *) = delete;
    static void operator delete[](void *) = delete;

    // The value being scoped.
    T m_value;

    // Pointers to the next and previous instances of the scoped class in the linked list of instances.
    scoped* m_next;
    scoped* m_prev;

    // Thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
    static thread_local scoped* s_top;
    static thread_local scoped* s_bottom;
};

// Define the thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
template<class T, class ...Tags>
thread_local scoped<T, Tags...>* scoped<T, Tags...>::s_top;

template<class T, class ...Tags>
thread_local scoped<T, Tags...>* scoped<T, Tags...>::s_bottom;

#endif // _INCLUDE_SCOPED_H_
