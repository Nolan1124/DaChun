#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "http_content.h"
#include "server.h"
namespace dachun {
Server::Server(EventLoop *loop, int num_threads, int port) 
      : loop_(loop),
        num_threads_(num_threads),
        thread_pool_(new EventLoopThreadPool(loop_, num_threads_)),
        started_(false),
        channel_(new Channel(loop_)),
        port_(port),
        listen_fd_(socket_bind_listen(port_))
{
  std::cout << "listen_fd_ is " << listen_fd_ << std::endl;
  channel_->set_fd(listen_fd_);
  //handle_for_sigpipe();
  if (SetSocketNonBlocking(listen_fd_) < 0) {
    perror("set socket non block failed");
    abort();
  }
  std::cout << "Server instance is established." << std::endl;
}

Server::~Server() {}

void Server::Start() {
  std::cout << "starting server's Start()." << std::endl;
  thread_pool_->start_pool();
  channel_->set_events(EPOLLIN | EPOLLET);
  channel_->set_read_handler(std::bind(&Server::HandNewConn, this));
  channel_->set_connect_handler(std::bind(&Server::HandThisConn, this));
  loop_->AddToPoller(channel_, 0);
  started_ = true;
}

void Server::HandNewConn() {
  struct sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_addr_len = sizeof(struct sockaddr_in);
  int accept_fd = 0;

  std::cout << "listen_fd:" << listen_fd_ << ".accept new connection." << std::endl;
  std::cout << "client_addr_len = " << client_addr_len << std::endl;
  while((accept_fd = accept(listen_fd_, (struct sockaddr*)&client_addr, &client_addr_len)) > 0) {
    std::cout << "while accept_fd =" << accept_fd << std::endl;
    EventLoop *loop = thread_pool_->getNextLoop();
    /*
    // TCP的保活机制默认是关闭的
    int optval = 0;
    socklen_t len_optval = 4;
    getsockopt(accept_fd, SOL_SOCKET,  SO_KEEPALIVE, &optval, &len_optval);
    cout << "optval ==" << optval << endl;
    */
    // 限制服务器的最大并发连接数
    if (accept_fd >= MAX_FD) {
      close(accept_fd);
      continue;
    }
    // 设为非阻塞模式
    if (SetSocketNonBlocking(accept_fd) < 0) {
      perror("Set non block failed!");
      return;
    }

    SetSocketNodelay(accept_fd);
    SetSocketNoLinger(accept_fd);

    std::shared_ptr<HttpContent> req_info(new HttpContent(loop, accept_fd));
    req_info->GetChannel()->set_holder(req_info);
    loop->QueueInLoop(std::bind(&HttpContent::NewEvent, req_info));
  }
  //std::cout << "accept fd = " << accept_fd << std::endl;
  //std::cout << "errno:" << errno << std::endl;
  channel_->set_events(EPOLLIN | EPOLLET);
}

void Server::HandThisConn() {
  loop_->ModifyPoller(channel_);
}
} // namespace dachun