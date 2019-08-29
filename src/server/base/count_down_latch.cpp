#include "count_down_latch.h"
namespace dachun {
CountDownLatch::CountDownLatch(int count)
    : mutex_(),
      condition_(mutex_),
      count_(count)
{ }

CountDownLatch::~CountDownLatch() { }

void CountDownLatch::Wait() {
  MutexLockGuard lock(mutex_);
  while (count_ > 0) {
    condition_.wait();
  }
}
void CountDownLatch::CountDown() {
  MutexLockGuard lock(mutex_);
  --count_;
  if (count_ == 0) {
    condition_.notifyAll();
  }
}
} // namespace dachun