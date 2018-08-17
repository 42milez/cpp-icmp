#ifndef ICMP_RAWSOCKET_H
#define ICMP_RAWSOCKET_H


#include <functional>
#include <memory>
#include <string>

#include <sys/epoll.h>

#include "spdlog/spdlog.h"

namespace network
{
  using std::function;
  using std::shared_ptr;
  using std::string;
  using std::vector;

  using spdlog::logger;

  class RawSocket {
  public:
    RawSocket(const string device);
    ~RawSocket();
    void wait();
  private:
    void create_socket();
    void setup_multiplexer();

    static const int NEVENTS;

    string device_;
    struct epoll_event ev_ret_[16];
    int fd_;
    int mux_;

    shared_ptr<logger> logger_;
  };
} // network


#endif // ICMP_RAWSOCKET_H
