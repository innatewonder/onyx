#include "CommonPrecompiled.h"
#include "Timer.h"

namespace Core
{
  Timer::Timer() : m_stopped(false)
  {
    Reset();
  }

  void Timer::Start()
  {
    if(m_stopped)
    {
      m_stopped = false;
      m_start = GameClock::now();
      m_step = m_start;
    }
    else
    {
      //offset the start time so total time is still correct
      TimePoint current = GameClock::now();
      m_start += (current - m_paused);
      m_step = m_start;
    }
  }

  void Timer::Step()
  {
    m_step = GameClock::now();
  }

  void Timer::Stop()
  {
    m_stopped = true;
  }

  void Timer::Pause()
  {
    m_paused = GameClock::now();
  }

  void Timer::Reset()
  {
    m_start = GameClock::now();
    m_paused = m_start;
    m_step = m_start;
  }

  f32 Timer::GetDT_Sec()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<SecDur>(current - m_step).count();
  }

  f32 Timer::GetDT_Milli()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<MilliDur>(current - m_step).count();
  }

  f32 Timer::GetDT_Micro()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<MicroDur>(current - m_step).count();
  }

  u64 Timer::UGetDT_Sec()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<USecDur>(current - m_step).count();
  }

  u64 Timer::UGetDT_Milli()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<UMilliDur>(current - m_step).count();
  }

  u64 Timer::UGetDT_Micro()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<UMicroDur>(current - m_step).count();
  }

  f64 Timer::GetTotalTimeSec()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<DSecDur>(current - m_start).count();
  }

  f64 Timer::GetTotalTimeMilli()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<DMilliDur>(current - m_start).count();
  }

  u64 Timer::UGetTotalTimeSec()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<USecDur>(current - m_start).count();
  }

  u64 Timer::UGetTotalTimeMilli()
  {
    TimePoint current = GameClock::now();
    return std::chrono::duration_cast<UMilliDur>(current - m_start).count();
  }
}
