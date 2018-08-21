#ifndef ICMP_RAWSOCKET_H
#define ICMP_RAWSOCKET_H


#include <functional>
#include <memory>
#include <string>

#if defined(__linux__)
  #include <sys/epoll.h>
#else
  // UNIX
  // ...
#endif

#include "spdlog/spdlog.h"

namespace network
{
  class RawSocket {
  public:
    RawSocket(const std::string device);
    ~RawSocket();
    void wait(std::function<void(const int fd)> fn);
  private:
    int create_socket();
    int setup_multiplexer();

    static const int NEVENTS;

    std::string device_;
#if defined(__linux__)
    struct epoll_event ev_ret_[16];
#else
    // UNIX
    // ...
#endif
    int fd_;
    int mux_;

    std::shared_ptr<spdlog::logger> logger_;
  };
} // network


#endif // ICMP_RAWSOCKET_H
