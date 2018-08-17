#ifndef ICMP_NETWORKIO_H
#define ICMP_NETWORKIO_H


#include <functional>

namespace network
{
  using std::function;
  using std::vector;

  class NetworkIO {
  public:
    NetworkIO(int port);
    ~NetworkIO();
    void wait_for_read(const function<void()>& fn);
  private:
    void create_socket();
    void setup_multiplexer();
    int fd_;
    int mux_;
    static const int NEVENTS;
    struct epoll_event ev_ret_[16];
  };
}


#endif // ICMP_NETWORKIO_H
