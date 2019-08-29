#ifndef BASE_COUNT_DOWN_LATCH_H_
#define BASE_COUNT_DOWN_LATCH_H_
#include "noncopyable.h"
#include "mutex_lock.h"
namespace dachun {
// 倒计时器  
class CountDownLatch : public NonCopyable {
 public:
  explicit CountDownLatch(int count);
  ~CountDownLatch();
  void Wait();
  void CountDown();
 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};
} // namespace dachun
#endif // BASE_COUNT_DOWN_LATCH_H_