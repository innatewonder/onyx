#pragma once

class TimeTester
{
  public:
    TimeTester();
    ~TimeTester();

    void Start();
    void End();

  private:
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

    TimePoint start_;
    TimePoint end_;
};
