#include <iostream>


#include "event_loop_thread_pool.h"
namespace dachun {
EventLoopThreadPool::EventLoopThreadPool(EventLoop* base_loop, int num_threads)
      : base_loop_(base_loop),
        started_(false),
        num_threads_(num_threads),
        next_(0)
{
  if (num_threads_ <= 0) {
    std::cerr << "error: the number of threads less than 0." << std::endl;
    abort();
  }
  std::cout << "ThreadPool instance is established." << std::endl;
}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::start_pool() {
  std::cout << "starting ThreadPool start_pool()." << std::endl; 
  base_loop_->AssertInLoopThread();
  started_ = true;
  for (int i = 0; i < num_threads_; i++) {
    std::shared_ptr<EventLoopThread> th(new EventLoopThread());
    threads_.push_back(th);
    loops_.push_back(th->StartLoop());
  }
  std::cout << "ThreadPool start success." << std::endl; 
}

EventLoop* EventLoopThreadPool::getNextLoop() {
  base_loop_->AssertInLoopThread();
  assert(started_);
  EventLoop* loop = base_loop_;
  if (!loops_.empty()) {
    loop = loops_[next_];
    next_ = (next_ + 1) % num_threads_; // 环形队列实现
  }

  std::cout << "getLoop=" << loop << std::endl;
  return loop;
}
} // namespace dachun