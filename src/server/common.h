#ifndef COMMON_H_
#define COMMON_H_
namespace dachun {
ssize_t readn(int fd, void *buffer, size_t n);
ssize_t readn(int fd, std::string &in_buffer, bool &zero);
ssize_t readn(int fd, std::string &in_buffer);
ssize_t writen(int fd, void *buffer, size_t n);
ssize_t writen(int fd, std::string &sbuff);

void shutdownWR(int fd);
int socket_bind_listen(int port);
int SetSocketNonBlocking(int fd);
void SetSocketNodelay(int fd);
void SetSocketNoLinger(int fd);
} // namespace dachun
#endif // COMMON_H_