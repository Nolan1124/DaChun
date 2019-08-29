#include <functional>

#include <unistd.h>
#include <sys/eventfd.h>

#include "common.h"
#include "event_loop.h"

namespace dachun {
__thread EventLoop* t_loop_in_this_thread = 0;

int CreateEventFd() {
  int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (event_fd < 0) {
    perror("create eventfd error.");
  }
  return event_fd;
}

EventLoop::EventLoop()
    : poller_(new Epoll()),
      looping_(false),
      wakeup_fd_(CreateEventFd()),
      thread_id_(CurrentThread::tid()),
      pending_functors_(0),
      calling_pending_functors_(false),
      event_handling_(false),
      wakeuped_channel_(new Channel(this, wakeup_fd_)) 
{
  if (!t_loop_in_this_thread){
    t_loop_in_this_thread = this;
  }
  wakeuped_channel_->set_events(EPOLLIN | EPOLLET);// 接受读事件,边缘触发
  wakeuped_channel_->set_read_handler(std::bind(&EventLoop::HandleRead, this));
  //wakeuped_channel_->set_connect_handler();
  poller_->epoll_add_event(wakeuped_channel_, 0);
}

EventLoop::~EventLoop() {
  close(wakeup_fd_);
  t_loop_in_this_thread = nullptr;
}

void EventLoop::Loop() {
  assert(!looping_);
  assert(IsInLoopThread());
  looping_ = true;
  std::vector<std::shared_ptr<Channel>> channel_list;
  while (looping_) {
    channel_list.clear();
    channel_list = poller_->poll(); // 封装poller的poll方法
    event_handling_ = true;
    for (auto &channel : channel_list) {
      channel->HandleEvents();
    }
    event_handling_ = false;
    DoPendingFunctors();
    //poller_->HandleExpired();
  }
  looping_ = false;
}
void EventLoop::RunInLoop(const Functor& cb) {
  // 如果在当前IO线程调用,则同步执行回调函数,否则将回调函数放入队列,并唤醒IO线程执行回调
  if (IsInLoopThread()) {
    cb();
  } else {
    QueueInLoop(cb);
  }
}
void EventLoop::QueueInLoop(const Functor& cb) {
  {
    MutexLockGuard lock(mutex_);
    pending_functors_.push_back(cb);
  }
  
  // 只有在IO线程的事件回调中调用该函数才无须唤醒
  if (!IsInLoopThread() || calling_pending_functors_) {
    std::cout << "QueueInLoop:wakeup()" << std::endl;
    Wakeup();
  }
}

void EventLoop::DeleteFromPoller(std::shared_ptr<Channel> channel) {
  poller_->epoll_delete_event(channel);
}
void EventLoop::ModifyPoller(std::shared_ptr<Channel> channel, int timeout) {
  poller_->epoll_modify_event(channel, timeout);
}
void EventLoop::AddToPoller(std::shared_ptr<Channel> channel, int timeout) {
  poller_->epoll_add_event(channel, timeout);
}

void EventLoop::shutdown(std::shared_ptr<Channel> channel) {
  shutdownWR(channel->fd());
}

void EventLoop::quit() {
  looping_ = false;
  if (!IsInLoopThread()) {
    Wakeup();
  }
}

// 利用eventfd通知读事件的发生
void EventLoop::HandleRead() {
  uint64_t one = 1;
  ssize_t n = read(wakeup_fd_, &one, sizeof(one));
  if (n != sizeof(one)) {
    perror("HandleRead:not 8 bytes.");
  }
  std::cout << "EventLoop:HandleRead,notify" << std::endl;
  wakeuped_channel_->set_events(EPOLLIN | EPOLLET);
}

// 利用eventfd唤醒事件
void EventLoop::Wakeup() {
  uint64_t one = 1;
  std::cout << "wakeupfd=" << wakeup_fd_ << std::endl;
  ssize_t n = write(wakeup_fd_, &one, sizeof(one));
  if (n != sizeof(one)) {
    perror("Wakeup:not 8 bytes.");
  }
  std::cout << "EventLoop:Wakeup" << std::endl;
}

void EventLoop::DoPendingFunctors()
{
    std::vector<Functor> functors;
    calling_pending_functors_ = true;

    {
        MutexLockGuard lock(mutex_);
        functors.swap(pending_functors_);// 成员变量转变为局部变量,避免死锁
    }

    for (size_t i = 0; i < functors.size(); ++i)
        functors[i]();
    calling_pending_functors_ = false;
}
} // namespace dachun