
#include <assert.h>

#include "channel.h"
#include "poller.h"

namespace dachun {
static const int kEventsNum = 4096;
static const int kEventWaitTime = 10000;

Epoll::Epoll()
    : epollfd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(kEventsNum) {
  assert(epollfd_ > 0);
}

Epoll::~Epoll() {}

void Epoll::epoll_add_event(SpChannel channel, int timeout){
  int fd = channel->fd();
  std::cout << "Poller:epoll_add_event:fd = " << fd << std::endl;
  if (timeout > 0) {
    // TODO:添加计时器
  }
  struct epoll_event event;
  event.data.fd = fd;
  event.events = channel->events();
  channel->set_last_event(event.events);
  // 将fd映射成对应channel
  channel_map_[fd] = channel;

  if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0) {
    perror("epoll_add_event error.\n");
    channel_map_[fd].reset();// 重置为空
  }
}

void Epoll::epoll_delete_event(SpChannel channel) {
  int fd = channel->fd();
  struct epoll_event event;
  event.data.fd = fd;
  event.events = channel->last_event();

  if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event) < 0) {
    perror("epoll_delete_event error.");
  }
  channel_map_[fd].reset();
  //http_map_[fd].reset();
}

void Epoll::epoll_modify_event(SpChannel channel, int timeout) {
  if (timeout > 0) {
    // TODO:add_timer()
  }
  int fd = channel->fd();
  if (!channel->IsLastEvent()) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = channel->events();
    channel->set_last_event(event.events);
    if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event)) {
      perror("epoll_modify_event error.");
      channel_map_[fd].reset();
    }
  }
}

// 封装epoll_wait,返回捕获到的活跃事件数
std::vector<std::shared_ptr<Channel>> Epoll::poll(){
  std::cout << "starting epoll wait" << std::endl;
  while (true) {
    int event_num = epoll_wait(epollfd_, &*events_.begin(), kEventsNum, kEventWaitTime);
    if (event_num < 0) {
      perror("epoll_wait error.");
      continue;
    }
    std::vector<std::shared_ptr<Channel>> events = GetEpollEvents(event_num);
    std::cout << "Poller:GetEpollEvents:size = "  << events.size() << std::endl;
    for (auto ev: events) {
      std::cout << "Poller:event_fd=" << ev->fd() << std::endl;
    }
    if (events.size() > 0) {
      return events;
    }

  }
}

std::vector<std::shared_ptr<Channel>> Epoll::GetEpollEvents(int event_num) const {
  std::vector<std::shared_ptr<Channel>> events;
  std::shared_ptr<Channel> current_channel;
  for (int i = 0; i < event_num; i++) {
    int fd = events_[i].data.fd;
    current_channel = channel_map_[fd];
    if (current_channel) {
      current_channel->set_current_event(events_[i].events);
      current_channel->set_events(0);//?
      events.push_back(current_channel);
    } else {
      perror("current channel is invalid.");
    }
  }
  return events;
}

} // namespace dachun