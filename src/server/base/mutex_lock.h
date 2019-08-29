#ifndef BASE_MUTEX_LOCK_H_
#define BASE_MUTEX_LOCK_H_
#include <pthread.h>
#include <errno.h>
#include "noncopyable.h"
namespace dachun {
class MutexLock : public NonCopyable{
 public:
  MutexLock();
  ~ MutexLock();
  void lock() { pthread_mutex_lock(&mutex_); }
  void unlock() { pthread_mutex_unlock(&mutex_); }
  pthread_mutex_t *mutex() { return &mutex_; } 

 private:
  pthread_mutex_t mutex_;
  friend class Condition;
};

class MutexLockGuard : public NonCopyable {
 public:
  explicit MutexLockGuard(MutexLock &mutex);
  ~ MutexLockGuard();
 private:
  MutexLock &mutex_;
};

class Condition : public NonCopyable {
 public:
  explicit Condition(MutexLock &mutex)
      : mutex_(mutex){
    pthread_cond_init(&cond_, NULL);
  }
  ~Condition() {
        pthread_cond_destroy(&cond_);
  }
  void wait() {
    pthread_cond_wait(&cond_, mutex_.mutex());
  }
  void notify() {
    pthread_cond_signal(&cond_);
  }
  void notifyAll() {
    pthread_cond_broadcast(&cond_);
  }
  bool waitForSeconds(int seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += static_cast<time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.mutex(), &abstime);
  }
private:
    MutexLock &mutex_;
    pthread_cond_t cond_;
};
} // namespace dachun
#endif // BASE_MUTEX_LOCK_H_