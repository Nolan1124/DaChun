#include <string>
#include <getopt.h>

#include "event_loop.h"
#include "server.h"

int main(int argc, char *argv[]) {
  int thread_num = 4;
  int port = 8080;
  //std::string logPath = "./WebServer.log";

  // parse args
  int opt;
  const char *str = "t:l:p:";
  while ((opt = getopt(argc, argv, str))!= -1) {
    switch (opt) {
      case 't': {
        thread_num = atoi(optarg);
        break;
      }
      case 'l': {
        /*logPath = optarg;
        if (logPath.size() < 2 || optarg[0] != '/') {
          printf("logPath should start with \"/\"\n");
          abort();
        }*/
        break;
      }
      case 'p': {
        port = atoi(optarg);
        break;
      }
      default: break;
    }
  }
  //Logger::setLogFileName(logPath);
    
  dachun::EventLoop main_loop;
  dachun::Server http_server(&main_loop, thread_num, port);
  http_server.Start();
  main_loop.Loop();
  return 0;
}