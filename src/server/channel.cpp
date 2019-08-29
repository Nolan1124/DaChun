#include <sys/epoll.h>


#include "event_loop.h"

#include "channel.h"

namespace dachun {

Channel::Channel(EventLoop *loop) 
    : fd_(0),
      loop_(loop),
      events_(0),
      current_event_(0),
      last_event_(0)
      //read_handler_(nullptr),
      //write_handler_(nullptr),
      //error_handler_(nullptr),
      //connect_handler_(nullptr)
      //holder_(0)
{}

Channel::Channel(EventLoop *loop, int fd) 
    : fd_(fd),
      loop_(loop),
      events_(0),
      current_event_(0),
      last_event_(0)
      //read_handler_(nullptr),
      //write_handler_(nullptr),
      //error_handler_(nullptr),
      //connect_handler_(nullptr)
      //holder_(0)
{}

Channel::~Channel() {}

void Channel::HandleReadEvents() {
  std::cout << "channel:starting HandleReadEvents" << std::endl;
  if (read_handler_) {
    std::cout << "excuting read_handler" << std::endl;
    read_handler_();
  }
    
}

void Channel::HandleWriteEvents() {
  if (write_handler_)
    write_handler_();
}

void Channel::HandleErrorEvents() {
  if (error_handler_)
    error_handler_();
}

void Channel::HandleConnectEvents() {
  if (connect_handler_)
    connect_handler_();
}

void Channel::HandleEvents() {
  if ((current_event_ & EPOLLHUP) && 
     !(current_event_ & EPOLLIN)) {
    events_ = 0;
    return;
  }
  if (current_event_ & EPOLLERR) {
    HandleErrorEvents();
    events_ = 0;
    return;
  }
  if (current_event_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
    std::cout << "Channel:HandleReadEvents" << std::endl;
    HandleReadEvents();
    //return;//?
  }
  if (current_event_ & EPOLLOUT) {
    HandleWriteEvents();
    //return;// ?
  }
  HandleConnectEvents();
}

} // namespace dachun