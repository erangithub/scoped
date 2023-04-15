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
#include <cassert>

namespace scoped
{

// An abstract class template for managing resources within a specific scope.
template <class T, class ...Tags>
class abstract_scoped
{
public:
    // Constructor that adds the current instance to the top of the linked list of instances.
    explicit abstract_scoped() : m_next(s_top), m_prev(nullptr)
    {
        if (s_bottom == nullptr) s_bottom = this;
        s_top = this;
        if (m_next) {
            m_next->m_prev = this;
        }
        assert(check_class_invariant());
        assert(check_instance_invariant());
    }

    // Destructor that removes the current instance from the linked list of instances.
    // Removal is properly handled even if the instances are not destructed in the
    // reverse order of their construction. This allows more flexibility of putting 
    // scoped objects in containers.
    ~abstract_scoped()
    {
        assert(check_instance_invariant());
        if (m_next) {
            m_next->m_prev = m_prev;
        } else {
            s_bottom = m_prev;
        }
        
        if (m_prev) {
            m_prev->m_next = this;
        } else {
            s_top = m_next;
        }
        m_next = m_prev = nullptr;
        assert(check_class_invariant());
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

    // Disable the use of the default new and delete operators, as scoped instances should not be created on the heap.
    static void* operator new(size_t) = delete;          // standard new
    static void* operator new[](size_t) = delete;        // array new
    static void* operator new(size_t, void*) = delete;   // placement new
    static void* operator new[](size_t, void*) = delete; // placement array new
    
    static void operator delete(void *) = delete;
    static void operator delete[](void *) = delete;

private:
    // Check the invariant of the linked list of instances: Either both s_top and s_bottom are nullptr,
    // or they are both non-null, and s_top->m_prev and s_bottom->m_next are also null.
    bool check_class_invariant() {
        assert((!s_top) == (!s_bottom));
        assert((!s_top) || (!s_top->m_prev));
        assert((!s_bottom) || (!s_bottom->m_next));
        return true;
    }

    // Check the invariant of the current instance:
    // - If m_prev is null, this instance must be s_top
    // - If m_next is null, this instance must be s_bottom
    bool check_instance_invariant() {
        assert((!m_prev) == (s_top == this));
        assert((!m_next) == (s_bottom == this));
        return true;
    }

    // Pointers to the next and previous instances of the scoped class in the linked list of instances.
    abstract_scoped* m_next;
    abstract_scoped* m_prev;

    // Thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
    static thread_local abstract_scoped* s_top;
    static thread_local abstract_scoped* s_bottom;
};

// Define the thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
template<class T, class ...Tags>
thread_local abstract_scoped<T, Tags...>* abstract_scoped<T, Tags...>::s_top = nullptr;

template<class T, class ...Tags>
thread_local abstract_scoped<T, Tags...>* abstract_scoped<T, Tags...>::s_bottom = nullptr;

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

} // namespace scoped

#endif // _INCLUDE_SCOPED_H_
