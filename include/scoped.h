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

template<class T, class ...Tags> class scoped_haven;

// An abstract class template for managing resources within a specific scope.
template <class T, class ...Tags>
class abstract_scoped {
public:
    using haven = scoped_haven<T, Tags...>;

    // Constructor that adds the current instance to the top of the linked list of instances.
    abstract_scoped() : m_next(nullptr), m_prev(nullptr) {
        insert(s_top);
        assert(check_class_invariant());
        assert(check_instance_invariant());
    }

    // Copy constructor that adds the current instance right above the other instance.
    // This helps maintain order stability, e.g. when vector<scoped> is resized.
    abstract_scoped(const abstract_scoped& other) : m_next(nullptr), m_prev(nullptr) {
        insert(const_cast<abstract_scoped*>(&other));
        assert(check_class_invariant());
        assert(check_instance_invariant());
        assert(other.check_instance_invariant());
    }

    // Copy assignement: the pointers have already been setup correctly in the default constructor,
    // make sure not to override them by copying from other.
    abstract_scoped& operator=(const abstract_scoped& other) {
        assert(check_class_invariant());
        assert(check_instance_invariant());
        assert(other.check_instance_invariant());
        return *this;
    }

    // Move constructor that replaces the current instance in the linked list of instances.
    abstract_scoped(abstract_scoped&& other) : m_next(nullptr), m_prev(nullptr) {
        insert(&other);
        other.detach();
        assert(check_class_invariant());
        assert(check_instance_invariant());
        assert(other.check_instance_invariant());
    }

    // Move assignment: the instance is already appended. Just detach the other instance.
    abstract_scoped& operator=(abstract_scoped&& other) {
        other.detach();
        assert(check_class_invariant());
        assert(check_instance_invariant());
        assert(other.check_instance_invariant());
        return *this;
    }

    // Destructor that removes the current instance from the linked list of instances.
    // Removal is properly handled even if the instances are not destructed in the
    // reverse order of their construction. This allows more flexibility of putting 
    // scoped objects in containers.
    virtual ~abstract_scoped() {
        detach();
        assert(check_class_invariant());
        assert(check_instance_invariant());
    }
    
    // Returns a reference to the value being scoped.
    virtual T& value() = 0;
 
    // Returns a pointer to the next instance of the scoped class in the linked list of instances.
    // Note: next() goes in the direction from top() to bottom()
    abstract_scoped* next() {
        return m_next;
    }

    // Returns a pointer to the previous instance of the scoped class in the linked list of instances.
    // Note: prev() goes in the direction from bottom() to top()
    abstract_scoped* prev() {
        return m_prev;
    }

    // Returns a pointer to the top instance of the scoped class in the linked list of instances.
    static abstract_scoped* top() {
        return s_top;
    }

    // Returns a pointer to the bottom instance of the scoped class in the linked list of instances.
    static abstract_scoped* bottom() {
        return s_bottom;
    }

    // Returns whether or not this instance is attached to the linked list of instances
    bool is_attached() {
        return m_next || m_prev || (s_top == this) || (s_bottom == this);
    }

    // Disable the use of the default new and delete operators, as scoped instances should not be created on the heap.
    static void* operator new(size_t) = delete;          // standard new
    static void* operator new[](size_t) = delete;        // array new
    static void* operator new(size_t, void*) = delete;   // placement new
    static void* operator new[](size_t, void*) = delete; // placement array new

private:
    void insert(abstract_scoped* above) {
        assert(!is_attached());
        if (above && !above->is_attached()) {
            above = s_top;
        }
        m_next = above;
        m_prev = above ? above->m_prev : s_bottom;
        if (m_prev) {
            m_prev->m_next = this;
        }
        else {
            s_top = this;
        }
        if (m_next) {
            m_next->m_prev = this;
        }
        else {
            s_bottom = this;
        }
    }

    void detach() {
        if (!is_attached()) return;
        if (m_next) {
            m_next->m_prev = m_prev;
        }
        if (m_prev) {
            m_prev->m_next = m_next;
        }
        if (s_top == this) {
            s_top = m_next;
        }
        if (s_bottom == this) {
            s_bottom = m_prev;
        }
        m_next = m_prev = nullptr;
    }

    // Check that the s_top and s_bottom are either fully detached, or property attached.
    bool check_class_invariant() const {
        assert((!s_top) == (!s_bottom));
        assert((!s_top) || (!s_top->m_prev));
        assert((!s_bottom) || (!s_bottom->m_next));
        return true;
    }

    // Check that this instance is either detached, or property attached.
    bool check_instance_invariant() const {
        assert((!m_next) || (m_next->m_prev == this));
        assert((!m_prev) || (m_prev->m_next == this));
        assert((s_top != this) || !m_prev);
        assert((s_bottom != this) || !m_next);
        assert(m_next || m_prev || ((s_top == this) && (s_bottom == this)) || ((s_top != this) && (s_bottom != this)));
        assert(!(!m_prev && m_next) || (s_top == this));
        assert(!(!m_next && m_prev) || (s_bottom == this));
        return true;
    }

    // Pointers to the next and previous instances of the scoped class in the linked list of instances.
    abstract_scoped* m_next;
    abstract_scoped* m_prev;

    // Thread-local storage for the top and bottom instances of the scoped class in the linked list of instances. 
    static thread_local abstract_scoped* s_top;
    static thread_local abstract_scoped* s_bottom;

    friend haven;
};

// Define the thread-local storage for the top and bottom instances of the scoped class in the linked list of instances.
template<class T, class ...Tags>
thread_local abstract_scoped<T, Tags...>* abstract_scoped<T, Tags...>::s_top = nullptr;

template<class T, class ...Tags>
thread_local abstract_scoped<T, Tags...>* abstract_scoped<T, Tags...>::s_bottom = nullptr;

// A class template for scoping values of type T, while interfacing them with the abstract scope for T's base class B.
template<class T, class B, class ...Tags> class polymorphic_scoped : public abstract_scoped<B, Tags...> {
public:
    using base = abstract_scoped<B, Tags...>;

    // Constructor that initializes the value being scoped with any number of arguments.
    template <class... Args>
    polymorphic_scoped(Args&&... args) : base(), m_value{std::forward<Args>(args)...}
    {}
    
    // Default contructors, destructor and assignment operators
    polymorphic_scoped(const polymorphic_scoped& other) = default;
    polymorphic_scoped(polymorphic_scoped&& other) = default;
    ~polymorphic_scoped() = default;
    polymorphic_scoped& operator=(const polymorphic_scoped& other) = default;
    polymorphic_scoped& operator=(polymorphic_scoped&& other) = default;

    B& value() override { return m_value; }

private:
    T m_value;
};

template<class T, class ...Tags> using scoped = polymorphic_scoped<T, T, Tags...>;

template<class T, class ...Tags> class scoped_haven {
public:
    using abstract = abstract_scoped<T, Tags...>;

    scoped_haven() : m_saved_top(abstract::top()),
                       m_saved_bottom(abstract::bottom()) {
        abstract::s_top = nullptr;
        abstract::s_bottom = nullptr;                   
    }

    ~scoped_haven() {
        abstract::s_top = m_saved_top;
        abstract::s_bottom = m_saved_bottom;
    }
    
private:
    abstract_scoped<T, Tags...>* m_saved_top;
    abstract_scoped<T, Tags...>* m_saved_bottom;
};

} // namespace scoped

#endif // _INCLUDE_SCOPED_H_
