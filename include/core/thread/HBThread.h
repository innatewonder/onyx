#pragma once

#include "ThreadJob.h"
#include "Semaphore.h"
#include "ThreadSafeQueue.h"
#include "ThreadSafeVar.h"

class ThreadPool
{
public:
  // 0 wll default to numCores
  ThreadPool(u16 numThreads = 0);
  ~ThreadPool();

  //this runs all the post execute commands on main thread
  void Update();
    
  /*WARNING: if you add the same pointer twice, it may break
  * same job is fine, just not same pointer, 
  * two threads can't run the same function ptr at the same time*/
  void AddJob(ThreadJob* j);
  void ClearJobs();

  u32  NumJobs();
  bool HasJobs();
  u32  NumCores() const;

  //Finishes all jobs completely
  void SyncPoint();

private:
  void ThreadMainFunc();
  
  Semaphore m_semaphore;
  std::vector<std::thread> m_threads;

  bool m_exitAll;
  ThreadSafeVar<unsigned> m_jobCount;

  ThreadSafeQueue<ThreadJob*> m_jobs;
  ThreadSafeQueue<ThreadJob*> m_finished;
};
