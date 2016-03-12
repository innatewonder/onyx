#include "CommonPrecompiled.h"
#include "Semaphore.h"

Semaphore::Semaphore()
  : m_count(0)
{}

void Semaphore::Clear()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_count = 0;
}

void Semaphore::Notify()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  ++m_count;
  lock.unlock();
  m_condition.notify_one();
}

void Semaphore::Notify(int num)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  m_count += num;
  m_condition.notify_all();
}

void Semaphore::Wait()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  m_condition.wait(lock, [this]{ return m_count > 0; });
  --m_count;
}
