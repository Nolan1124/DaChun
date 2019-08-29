#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_
#include <memory>

#include "event_loop.h"
#include "event_loop_thread_pool.h"

namespace dachun {
class Server {
 public:
  Server(EventLoop *loop, int num_threads, int port);
  ~Server();
  EventLoop* get_loop() const { return loop_; }
  void Start();
  void HandNewConn();
  void HandThisConn();

 private:
  EventLoop* loop_;
  int num_threads_;
  std::unique_ptr<EventLoopThreadPool> thread_pool_;
  bool started_;
  std::shared_ptr<Channel> channel_;
  int port_;
  int listen_fd_;
  static const int MAX_FD = 100000;
};

} // namespace dachun
#endif // SERVER_SERVER_H_