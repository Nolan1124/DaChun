#ifndef SERVER_CHANNEL_H_
#define SERVER_CHANNEL_H_
#include <iostream>
#include <functional>
#include <memory>

namespace dachun {
class EventLoop; // 前置声明
class HttpContent;

// 事件分发与处理类
class Channel {
 public:
  explicit Channel(EventLoop *loop);
  Channel(EventLoop *loop, int fd);
  ~Channel();

  void set_fd(int fd) { fd_ = fd; }
  int fd() const { return fd_; }

  void set_events(unsigned int events) { events_ = events; }
  unsigned int &events() { return events_; }

  void set_current_event(unsigned int current_event) {
    current_event_ = current_event;
  }
  unsigned int current_event() const { return current_event_; }

  void set_last_event(unsigned int last_event) { last_event_ = last_event; }
  unsigned int last_event() const { return last_event_; }
  
  void set_holder(std::shared_ptr<HttpContent> holder) {
    holder_ = holder;
  }
  std::shared_ptr<HttpContent> holder() const {
    std::shared_ptr<HttpContent> hd(holder_.lock());
    return hd;
  }

  void set_read_handler(std::function<void()> &&read_handler) {
    read_handler_ = read_handler;
    std::cout << "read_handler=" << &read_handler_ << std::endl;
  }
  void set_write_handler(std::function<void()> &&write_handler) {
    write_handler_ = write_handler;
  }
  void set_error_handler(std::function<void()> &&error_handler) {
    error_handler_ = error_handler;
  }
  void set_connect_handler(std::function<void()> &&connect_handler) {
    connect_handler_ = connect_handler;
  }

  bool IsLastEvent() const { return last_event_ == events_; }
  //void UpdateLastEvent() { last_event_ = events_; }

  void HandleEvents();
  void HandleReadEvents();
  void HandleWriteEvents();
  void HandleErrorEvents();
  void HandleConnectEvents();

 private:
  int fd_;
  EventLoop *loop_;
  unsigned int events_;
  unsigned int current_event_;
  unsigned int last_event_;

  std::weak_ptr<HttpContent> holder_;
  
  // 不同事件的回调函数声明
  typedef std::function<void()> CallBack;
  CallBack read_handler_;
  CallBack write_handler_;
  CallBack error_handler_;
  CallBack connect_handler_;
};

typedef std::shared_ptr<Channel> SpChannel;
} // namespace dachun
#endif // SERVER_CHANNEL_H_