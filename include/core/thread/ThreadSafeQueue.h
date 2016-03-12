#pragma once

#include <queue>
#include <mutex>

template<typename T>
class ThreadSafeQueue
{
public:
  ~ThreadSafeQueue();

  bool Empty();
  void Clear();
  void ClearWithDelete();
  unsigned Size();

  void Push(T var);
  T Pop();

private:
  typedef std::lock_guard<std::mutex> ScopeGuard;

  std::queue<T> m_queue;
  std::mutex    m_lock;
};

#include "ThreadSafeQueue.hpp"
