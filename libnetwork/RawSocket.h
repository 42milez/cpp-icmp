#ifndef ICMP_RAWSOCKET_H
#define ICMP_RAWSOCKET_H


#include <functional>

#include "Socket.h"

namespace network
{
  using std::function;
  using std::vector;

  class RawSocket : public Socket {
  public:
    RawSocket();
    ~RawSocket();
    void wait() override;
  private:
    void create_socket() override;
    void setup_multiplexer() override;
    int fd_;
    int mux_;
    static const int NEVENTS;
    struct epoll_event ev_ret_[16];
  };
} // network


#endif // ICMP_RAWSOCKET_H
