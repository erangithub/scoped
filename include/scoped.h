#ifndef _INCLUDE_SCOPED_H_
#define _INCLUDE_SCOPED_H_

#include <utility>

template <class T, class ...Tags> class scoped
{
public:
    template <class... Args>
    explicit scoped(Args&&... args) : m_value{std::forward<Args>(args)...}, m_next(s_top) {
        if (s_bottom == nullptr) s_bottom = this;
        s_top = this;
        if (m_next) m_next->m_prev = this;
    }

    ~scoped() {
        if (m_next) m_next->m_prev = nullptr;
        s_top = m_next;
        if (s_bottom == this) s_bottom = nullptr;
    }

    T& value() { return m_value; }
 
    scoped* next() { return m_next; }

    scoped* prev() { return m_prev; }
    
    static scoped* top() { return s_top; }

    static scoped* bottom() { return s_bottom; }

    static T* get() { return s_top ? &(s_top->m_value) : nullptr; }

private:
    static void * operator new(std::size_t) = delete;
    static void * operator new [] (std::size_t) = delete;

    T m_value;
    scoped* m_next;
    scoped* m_prev;

    static thread_local scoped* s_top;
    static thread_local scoped* s_bottom;
};

template<class T, class ...Tags> thread_local scoped<T, Tags...>* scoped<T, Tags...>::s_top;
template<class T, class ...Tags> thread_local scoped<T, Tags...>* scoped<T, Tags...>::s_bottom;

#endif // _INCLUDE_SCOPED_H_