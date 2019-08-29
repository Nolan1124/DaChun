#include "mutex_lock.h"

namespace dachun {
MutexLock::MutexLock() {
  pthread_mutex_init(&mutex_, nullptr);
}
MutexLock::~ MutexLock() {
  pthread_mutex_destroy(&mutex_);
}

MutexLockGuard::MutexLockGuard(MutexLock &mutex)
    : mutex_(mutex) {
        mutex_.lock();
}
MutexLockGuard::~MutexLockGuard() {
        mutex_.unlock();
}
} // namespace dachun