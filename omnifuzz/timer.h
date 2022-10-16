#ifndef OMNIFUZZ_TIMER_H
#define OMNIFUZZ_TIMER_H

#include <cstdint>
#include <chrono>

namespace omnifuzz {

class TimerImpl {
 public:
  virtual ~TimerImpl() = default;
  virtual void DevStart(void) = 0;
  virtual uint64_t DevStop(void) = 0;
};

class Timer {
 public:
  Timer();
  ~Timer();
  void Start(void);
  uint64_t Stop(void);
 protected:
  TimerImpl* timer_impl_;  
};

class ChronoSteadyTimerImpl : public TimerImpl {
  using timep_t = typename std::chrono::steady_clock::time_point;
 public:
  ChronoSteadyTimerImpl();
  virtual ~ChronoSteadyTimerImpl();
  virtual void DevStart(void) override;
  virtual uint64_t DevStop(void) override;
 private:
  std::chrono::steady_clock::time_point start_, end_;
};



}

#endif  // OMNIFUZZ_TIMER_H
