#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

const int kServerPort = 8080;
const char *kIPAddress = "127.0.0.1";

int setSocketNonBlocking1(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  if(flag == -1)
    return -1;

  flag |= O_NONBLOCK;
  if(fcntl(fd, F_SETFL, flag) == -1)
    return -1;
  return 0;
}

int main() {
  int sockfd;
  char buffer[4096];
  struct sockaddr_in server_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  std::cout << "sockfd:" << sockfd << std::endl;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(kServerPort);
  //inet_pton(AF_INET, kIPAddress, &server_addr.sin_addr);
  server_addr.sin_addr.s_addr = inet_addr(kIPAddress);
  buffer[0] = '\0';

  //测试1：发空串测试
   const char *buf = " ";
   if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
    //setSocketNonBlocking1(sockfd);
    std::cout << "test 1: empty string:" << std::endl; 
    ssize_t n = write(sockfd, buf, strlen(buf));
    std::cout << "strlen(buf) = " << strlen(buf) << std::endl;
    sleep(2);

    n = read(sockfd, buffer, 4096);
    std::cout << "n=" << n << std::endl;
    printf("recv buffer:%s\n", buffer);
    close(sockfd);
  } else {
    perror("test1:connect error.");
  }
  sleep(2);

 // 测试2： 发以下内容
  buf = "GET    HTTP/1.1";
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
    setSocketNonBlocking1(sockfd);
    std::cout << "test 2:" << std::endl; 
    ssize_t n = write(sockfd, buf, strlen(buf));
    std::cout << "strlen(buf) = " << strlen(buf) << std::endl;
    sleep(2);
    n = read(sockfd, buffer, 4096);
    std::cout << "n=" << n << std::endl;
    printf("recv buffer:%s\n", buffer);
    close(sockfd);
  } else {
    perror("test2:connect error.");
  }
  sleep(2);

  // 测试3，发一下内容
  buf = "GET / HTTP/1.1\r\nHost: 192.168.1.109:8080\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: Keep-Alive\r\n\r\n";
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
    setSocketNonBlocking1(sockfd);
    std::cout << "test 3:" << std::endl; 
    ssize_t n = write(sockfd, buf, strlen(buf));
    std::cout << "strlen(buf) = " << strlen(buf) << std::endl;
    sleep(1);
    n = read(sockfd, buffer, 4096);
    std::cout << "n=" << n << std::endl;
    printf("recv buffer:%s\n", buffer);
    close(sockfd);
  } else {
    perror("test3:connect error.");
  }
  return 0;
}





