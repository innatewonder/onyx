#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
  Semaphore();

  //resets count to 0
  void Clear();

  void Notify();
  void Notify(int num);
  void Wait();

private:
  std::mutex m_mutex;
  std::condition_variable m_condition;
  int m_count;
};
