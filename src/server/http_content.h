#ifndef SERVER_HTTP_CONTENT_H_
#define SERVER_HTTP_CONTENT_H_
#include <unordered_map>
#include <map>
#include <memory>

#include <sys/epoll.h>
#include <unistd.h>

#include "event_loop.h"

namespace dachun {
/* 状态定义 */
enum ProcessState
{
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum URIState
{
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS
};

enum HeaderState
{
    PARSE_HEADER_SUCCESS = 1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum AnalysisState
{
    ANALYSIS_SUCCESS = 1,
    ANALYSIS_ERROR
};

enum ParseState
{
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

enum ConnectionState
{
    H_CONNECTED = 0,
    H_DISCONNECTING,
    H_DISCONNECTED    
};

enum HttpMethod
{
    METHOD_POST = 1,
    METHOD_GET,
    METHOD_HEAD
};

enum HttpVersion
{
    HTTP_10 = 1,
    HTTP_11
};

/* HTTP元数据信息 */
class MimeType {
 private:
    static void init();
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    explicit MimeType(const MimeType &m);

 public:
    static std::string get_mime(const std::string &suffix);

private:
    static pthread_once_t once_control;
};

// HTTP解析类
class HttpContent : public std::enable_shared_from_this<HttpContent> {
 public:
    HttpContent(EventLoop *loop, int connfd);
    ~HttpContent() { close(fd_); }
    void reset();
    //void seperateTimer();
    /*void linkTimer(std::shared_ptr<TimerNode> mtimer)
    {
        // shared_ptr重载了bool, 但weak_ptr没有
        timer_ = mtimer; 
    }*/
    std::shared_ptr<Channel> GetChannel() { return channel_; }
    EventLoop *GetLoop() { return loop_; }
    void HandleClose();
    void NewEvent();
 private:
    EventLoop *loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string in_buffer_;
    std::string out_buffer_;
    bool error_;
    ConnectionState connectionState_;

    HttpMethod method_;
    HttpVersion version_;
    std::string file_name_;
    std::string path_;
    int cur_read_pos_;
    ProcessState pr_state_;
    ParseState pa_state_;
    bool keep_alive_;
    std::map<std::string, std::string> headers_;
    //std::weak_ptr<TimerNode> timer_;

    void HandleRead();
    void HandleWrite();
    void HandleConn();
    void HandleError(int fd, int err_num, std::string short_msg);
    URIState ParseURI();
    HeaderState ParseHeaders();
    AnalysisState AnalysisRequest();
};
} // namespace dachun
#endif // SERVER_HTTP_CONTENT_H_