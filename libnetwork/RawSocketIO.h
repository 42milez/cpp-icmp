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
  using std::function;
  using std::shared_ptr;
  using std::string;
  using std::vector;

  using spdlog::logger;

  class RawSocketIO {
  public:
    RawSocketIO(const string device);
    ~RawSocketIO();
    void wait(function<void()> fn);
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


#endif // ICMP_RAWSOCKETIO_H
