
template<typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue()
{
  Clear();
}

template<typename T>
bool ThreadSafeQueue<T>::Empty()
{
  //this likely isn't needed, but just in case
  ScopeGuard lock(m_lock);

  return m_queue.empty();
}

template<typename T>
void ThreadSafeQueue<T>::Clear()
{
  ScopeGuard lock(m_lock);
  
  std::queue<T> empty;
  std::swap(m_queue, empty);
}

template<typename T>
void ThreadSafeQueue<T>::ClearWithDelete()
{
  ScopeGuard lock(m_lock);

  while(!m_queue.empty())
  {
    delete m_queue.front();
    m_queue.pop();
  }
}

template<typename T>
unsigned ThreadSafeQueue<T>::Size()
{
  ScopeGuard lock(m_lock);

  return m_queue.size();
}

template<typename T>
void ThreadSafeQueue<T>::Push(T var)
{
  ScopeGuard lock(m_lock);

  m_queue.push(var);
}

template<typename T>
T ThreadSafeQueue<T>::Pop()
{
  ScopeGuard lock(m_lock);

  T ret = m_queue.front();
  m_queue.pop();
  return ret;
}

