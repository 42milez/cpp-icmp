#ifndef ICMP_RAWSOCKETIO_H
#define ICMP_RAWSOCKETIO_H


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
  class RawSocketIO {
  public:
    RawSocketIO(const std::string device);
    ~RawSocketIO();
    void wait(std::function<void()> fn);
  private:
    void create_socket();
    void setup_multiplexer();

    static const int NEVENTS;

    std::string device_;
    struct epoll_event ev_ret_[16];
    int fd_;
    int mux_;

    std::shared_ptr<spdlog::logger> logger_;
  };
} // network


#endif // ICMP_RAWSOCKETIO_H
