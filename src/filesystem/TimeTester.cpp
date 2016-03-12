#include "CommonPrecompiled.h"
#include "TimeTester.h"

TimeTester::TimeTester()
{}

TimeTester::~TimeTester()
{}

void TimeTester::Start()
{
  std::cout << "=====Starting Test=====" << std::endl;
  start_ = Clock::now();
}

void TimeTester::End()
{
  end_ = Clock::now();
  std::cout << "======Ending Test======" << std::endl;
  std::cout << "  Time Elapsed:" << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() << std::endl;
}
