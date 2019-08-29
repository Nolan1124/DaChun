#ifndef SERVER_EVENT_LOOP_THREAD_H_
#define SERVER_EVENT_LOOP_THREAD_H_

//#include "base/mutex_lock.h"
#include "base/noncopyable.h"
#include "event_loop.h"
#include "base/thread.h"

namespace dachun {
class EventLoopThread : public NonCopyable {
 public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* StartLoop();

 private:
  EventLoop *loop_;
  bool exiting_;
  MutexLock mutex_;
  Condition cond_;
  Thread thread_;

  void ThreadFunc();
};
} // namespace dachun
#endif // SERVER_EVENT_LOOP_THREAD_H_