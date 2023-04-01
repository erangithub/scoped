#ifndef _INCLUDE_SCOPED_H_
#define _INCLUDE_SCOPED_H_

#include <cstddef>

template <class T, class ...Tags> class scoped
{
public:
    scoped() : m_next(s_top) {
        s_top = this;
    }

    scoped(const T& data) : m_value(data), m_next(s_top) {
        s_top = this;
    }

    scoped(T&& data) : m_value(data), m_next(s_top) {
        s_top = this;
    }

    ~scoped() {
        s_top = m_next;
    }

    T& value() { return m_value; }
 
    scoped* next() { return m_next; };
    
    static scoped* top() { return s_top; }

    static T* get() { return s_top ? &(s_top->m_value) : nullptr; }

private:
    static void * operator new(std::size_t) = delete;
    static void * operator new [] (std::size_t) = delete;

    T m_value;
    scoped* m_next;

    static thread_local scoped* s_top;
};

template<class T, class ...Tags> thread_local scoped<T, Tags...>* scoped<T, Tags...>::s_top;

#endif // _INCLUDE_SCOPED_H_