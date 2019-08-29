#include <iostream>
#include <memory>

#include <unistd.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <assert.h>

#include "current_thread.h"
#include "thread.h"
namespace dachun {
namespace CurrentThread {
  __thread int t_cached_tid = 0;
  __thread char t_tid_string[32];
  __thread int t_tid_length = 6;
  __thread const char* t_thread_name = "unknown";
} // namespace CurrentThread

// 通过系统调用获取唯一tid
pid_t gettid() {
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void *startThread(void* obj) {
  Thread* thread = static_cast<Thread*>(obj);
  thread->runInThread();
  return nullptr;
}

void CurrentThread::CacheTid() {
  if (t_cached_tid == 0) {
    t_cached_tid = gettid();
    t_tid_length = snprintf(t_tid_string, sizeof(t_tid_string), "%5d ", t_cached_tid);
  }
}

Thread::Thread(const ThreadFunc& func, const std::string& name)
    : started_(false),
      joined_(false),
      pthread_id_(0),
      tid_(0),
      func_(func),
      name_(name),
      latch_(1)
{ 
  SetDefaultName();
}

Thread::~Thread() {
  if (started_ && ! joined_) {
    pthread_detach(pthread_id_);
  }
}

void Thread::SetDefaultName() {
  if (name_.empty()) {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread");
    name_ = buf;
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;
  errno = pthread_create(&pthread_id_, nullptr, &startThread, this);
  if (errno != 0) {
    perror("pthread_create error.");
  }
}

void Thread::runInThread() {
  tid_ = CurrentThread::tid();
  CurrentThread::t_thread_name = name_.c_str();
  try {
    func_();
    CurrentThread::t_thread_name = "finished";
  } catch (const std::exception& e) {
    CurrentThread::t_thread_name = "crashed";
    std::cout << e.what() << std::endl;
    abort();
  }
}

int Thread::join() {
  assert(started_);
  return pthread_join(pthread_id_, nullptr);
}
} // namespace dachun