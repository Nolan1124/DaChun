#ifndef SERVER_POLLER_H_
#define SERVER_POLLER_H_
#include <vector>
#include <sys/epoll.h>
#include <memory>

#include "channel.h"

namespace dachun {
//Linux Epoll机制的封装类
class Epoll {
 public:
  Epoll();
  ~Epoll();

  // 对epoll事件进行添加/修改/删除操作
  void epoll_add_event(SpChannel channel, int timeout);
  void epoll_modify_event(SpChannel channel, int timeout);
  void epoll_delete_event(SpChannel channel);

  // poll循环返回epoll捕获的活跃事件数
  std::vector<std::shared_ptr<Channel>> poll();
  
 private:
  static const int kMaxFds = 10000;
  int epollfd_;
  std::vector<struct epoll_event> events_;
  std::shared_ptr<Channel> channel_map_[kMaxFds];

  std::vector<std::shared_ptr<Channel>> GetEpollEvents(int event_num) const;
};


} // namespace dachun
#endif // SERVER_POLLER_H_