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
    RawSocket(const std::string& device);
    ~RawSocket();
    int fd();
  private:
    void create_socket();

#if defined(__linux__)
    struct epoll_event ev_ret_[16];
#else
    // UNIX
    // ...
#endif

    std::shared_ptr<spdlog::logger> logger_;
    std::string device_;
    int fd_;
    int mux_;
  };
} // network


#endif // ICMP_RAWSOCKET_H
