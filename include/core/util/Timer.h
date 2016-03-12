#pragma once

#include <chrono>
namespace Core
{
  class Timer
  {
  public:
    typedef std::chrono::high_resolution_clock GameClock;
    typedef std::chrono::time_point<GameClock> TimePoint;

    typedef std::chrono::duration<u64, std::milli> UMilliDur;
    typedef std::chrono::duration<u64, std::micro> UMicroDur;
    typedef std::chrono::duration<u64, std::ratio<1>> USecDur;

    typedef std::chrono::duration<f32, std::milli> MilliDur;
    typedef std::chrono::duration<f32, std::micro> MicroDur;
    typedef std::chrono::duration<f32, std::ratio<1>> SecDur;

    typedef std::chrono::duration<f64, std::ratio<1>> DSecDur;
    typedef std::chrono::duration<f64, std::milli> DMilliDur;

  public:
    Timer();

    void Start();

    //while the timer is going, saves current time for next dt
    //allows calls to getDT to all be same on same frame
    void Step();

    void Stop();
    void Pause();
    void Reset();

    f32 GetDT_Sec();
    f32 GetDT_Milli();
    f32 GetDT_Micro();

    u64 UGetDT_Sec();
    u64 UGetDT_Milli();
    u64 UGetDT_Micro();

    f64 GetTotalTimeSec();
    f64 GetTotalTimeMilli();

    u64 UGetTotalTimeSec();
    u64 UGetTotalTimeMilli();

  private:
    TimePoint m_start;
    TimePoint m_paused;

    TimePoint m_step;

    bool m_stopped;
  };
}
