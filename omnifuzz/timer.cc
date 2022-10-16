#include <iostream>
#include "omnifuzz/timer.h"

namespace omnifuzz {

Timer::Timer() {
  timer_impl_ = new ChronoSteadyTimerImpl; 
}

Timer::~Timer() {
  if (timer_impl_) {
    delete timer_impl_;
  }
}

void Timer::Start(void) {
  timer_impl_->DevStart();
};

uint64_t Timer::Stop(void) {
  return timer_impl_->DevStop();
}

ChronoSteadyTimerImpl::ChronoSteadyTimerImpl() {
}

ChronoSteadyTimerImpl::~ChronoSteadyTimerImpl() {}

void ChronoSteadyTimerImpl::DevStart(void) {
  start_ = std::chrono::steady_clock::now();
}

uint64_t ChronoSteadyTimerImpl::DevStop(void) {
  end_ = std::chrono::steady_clock::now();
  auto diff = end_ - start_;
  return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
}

}
