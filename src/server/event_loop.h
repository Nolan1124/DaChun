#ifndef SERVER_EVENT_LOOP_H_
#define SERVER_EVENT_LOOP_H_
#include <memory>


#include <assert.h>
#include <pthread.h>

#include "base/mutex_lock.h"
#include "base/current_thread.h"
#include "poller.h"

namespace dachun {
// 事件循环类,遵循one loop per thread原则    
class EventLoop {
 public:
 typedef std::function<void()> Functor;
  EventLoop();
  ~ EventLoop();

  void Loop();
  // 线程处理活跃事件
  void RunInLoop(const Functor& cb);
  // 活跃事件等待处理
  void QueueInLoop(const Functor& cb);
  bool IsInLoopThread() const { return thread_id_ == CurrentThread::tid(); }
  void AssertInLoopThread() { assert(IsInLoopThread()); }

  void DeleteFromPoller(std::shared_ptr<Channel> channel);
  void ModifyPoller(std::shared_ptr<Channel> channel, int timeout = 0);
  void AddToPoller(std::shared_ptr<Channel> channel, int timeout = 0);

  void quit();
  void shutdown(std::shared_ptr<Channel> channel);

 private:
  std::shared_ptr<Epoll> poller_;
  bool looping_;
  int wakeup_fd_;
  pthread_t thread_id_;

  std::vector<Functor> pending_functors_;
  bool calling_pending_functors_;
  std::shared_ptr<Channel> wakeuped_channel_;
  MutexLock mutex_;
  bool event_handling_;

  void HandleRead();
  void Wakeup();
  // 调用回调列表中的函数
  void DoPendingFunctors();

};
} // namespace dachun
#endif // SERVER_EVENT_LOOP_H_