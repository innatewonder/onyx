#pragma once

class ThreadJob
{
public:
  ThreadJob(bool postOnMainThread = false)
    : m_postMainThread(postOnMainThread)
  {}

  virtual void Execute() {};
  virtual void PostExecute() {};

  bool IsPostOnMainThread()
  {
    return m_postMainThread;
  }

private:
  bool m_postMainThread;
};
