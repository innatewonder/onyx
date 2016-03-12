#pragma once

#include <mutex>

//this will only be safe if it never returns a pointer
//never returns reference, as that wouldn't be safe
template<typename T>
class ThreadSafeVar
{
#define BINARY_OPERATOR(ret, op) \
  ret operator op(const T& rhs) \
  { \
    ScopeGuard lock(m_mutex); \
    m_data op rhs; \
    return m_data; \
  }

#define COMPARISON_OPERATOR(op) \
  bool operator op(T& rhs) \
  { \
    ScopeGuard lock(m_mutex); \
    return m_data op rhs; \
  }

#define PRE_OPERATOR(ret, op) \
  ret operator op() \
  { \
    ScopeGuard lock(m_mutex); \
    op m_data; \
    return m_data; \
  }

  public:
    ThreadSafeVar(T data)
      : m_data(data) {}

    BINARY_OPERATOR(T, =)
    BINARY_OPERATOR(T, +)
    BINARY_OPERATOR(T, *)
    BINARY_OPERATOR(T, /)
    BINARY_OPERATOR(T, %)
    
    PRE_OPERATOR(T, ++)
    PRE_OPERATOR(T, -)
    PRE_OPERATOR(T, --)

    COMPARISON_OPERATOR(==)
    COMPARISON_OPERATOR(!=)
    COMPARISON_OPERATOR(>)
    COMPARISON_OPERATOR(<)
    COMPARISON_OPERATOR(>=)
    COMPARISON_OPERATOR(<=)

    T operator!()
    {
      ScopeGuard lock(m_mutex);
      return !m_data;
    }

    bool operator&&(T& rhs)
    {
      ScopeGuard lock(m_mutex);
      return m_data && rhs;
    }

    bool operator||(T& rhs)
    {
      ScopeGuard lock(m_mutex);
      return m_data || rhs;
    }

    //cast to type
    operator T()
    {
      ScopeGuard lock(m_mutex);
      return m_data;
    }

    T& operator *()
    {
      ScopeGuard lock(m_mutex);
      return m_data;
    }

  private:
    T m_data;

    typedef std::lock_guard<std::mutex> ScopeGuard;
    std::mutex m_mutex;
};

