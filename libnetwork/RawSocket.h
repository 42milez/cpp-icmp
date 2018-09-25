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
    RawSocket(const std::string& device, const std::string &logger_name);
    ~RawSocket();
    int fd();
  private:
    void create_socket();
    std::string device_;
    int fd_;
    int mux_;
  };
} // network


#endif // ICMP_RAWSOCKET_H
