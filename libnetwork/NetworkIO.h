#ifndef ICMP_NETWORKIO_H
#define ICMP_NETWORKIO_H


#include <functional>

namespace network
{
  using std::function;

  class NetworkIO {
  public:
    NetworkIO(int port);
    ~NetworkIO();
    void wait_for_read(const function<void()>& fn);
  private:
    void create_raw_socket();
    void create_multiplexer();
    int soc_;
    int mux_;
    int port_;
  };
}


#endif // ICMP_NETWORKIO_H
