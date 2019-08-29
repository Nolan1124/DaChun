#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_
#include <string>
#include <functional>
#include "noncopyable.h"
#include "count_down_latch.h"

namespace dachun {
class Thread : public NonCopyable {
 public:
  typedef std::function<void()> ThreadFunc;
  explicit Thread(const ThreadFunc&, const std::string& name = std::string());
  ~Thread();
  void start();
  int join();
  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  const std::string& name() const { return name_; }

  void runInThread();

 private:
  bool started_;
  bool joined_;
  pthread_t pthread_id_;
  pid_t tid_;
  ThreadFunc func_;
  std::string name_;
  CountDownLatch latch_;

  void SetDefaultName();
};
} // namespace dachun
#endif // BASE_THREAD_H_