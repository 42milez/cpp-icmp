#ifndef ICMP_SOCKET_H
#define ICMP_SOCKET_H


#include <functional>
#include <vector>

namespace network
{
  using std::function;
  using std::vector;

  class Socket {
  public:
    virtual ~Socket();
    virtual void wait() = 0;
  protected:
    virtual void create_socket() = 0;
    virtual void setup_multiplexer() = 0;
    int fd_;
    int mux_;
    static const int NEVENTS;
    struct epoll_event ev_ret_[16];
  };
} // network


#endif // ICMP_SOCKET_H
