
#include <functional>

#include "event_loop_thread.h"
namespace dachun {
EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      exiting_(false),
      mutex_(),
      cond_(mutex_),
      thread_(std::bind(&EventLoopThread::ThreadFunc, this), "EventLoopThread")
{

}

EventLoopThread::~EventLoopThread() {
  exiting_ = true;
  if (loop_ != nullptr) {
    loop_->quit();
    thread_.join();
  }
}

EventLoop* EventLoopThread::StartLoop() {
  assert(!thread_.started());
  thread_.start();
  {
    MutexLockGuard lock(mutex_);
    while (loop_ == nullptr) {
      cond_.wait();
    } // 使用条件变量一直等待到threadFunc真正运行起来
  }
  return loop_;
}

void EventLoopThread::ThreadFunc() {
  EventLoop loop;
  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop; // 初始化loop对象，并使loop_指针指向该对象
    cond_.notify(); 
  }
  loop.Loop();
  
  // 线程执行结束，将间接指针置空
  loop_ = nullptr;
}

} // namespace dachun