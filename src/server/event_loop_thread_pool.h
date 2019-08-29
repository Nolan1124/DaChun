#ifndef SERVER_EVENT_LOOP_THREAD_POOL_H_
#define SERVER_EVENT_LOOP_THREAD_POOL_H_

#include "event_loop_thread.h"

namespace dachun {
class EventLoopThreadPool {
 public:
  EventLoopThreadPool(EventLoop *base_loop, int num_threads);
  ~ EventLoopThreadPool();

  /*  启动线程池 */ 
  void start_pool();
  /* 获取要处理的下一个任务 */
  EventLoop* getNextLoop();
 private:
  EventLoop* base_loop_;
  bool started_;
  int num_threads_;
  int next_;
  std::vector<std::shared_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};
} // namespace dachun
#endif // SERVER_EVENT_LOOP_THREAD_POOL_H_