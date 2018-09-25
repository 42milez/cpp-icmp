#ifndef ICMP_TCPSOCKET_H
#define ICMP_TCPSOCKET_H

#include <functional>
#include <memory>
#include <string_view>

#ifdef __linux__
#include <sys/epoll.h>
#else
// unix
// ...
#endif

# include <spdlog/spdlog.h>

namespace network
{
  class TcpSocket {
  public:
    TcpSocket(std::string_view device);
    ~TcpSocket();
    int fd();
  private:
    void create_socket(int port);

    std::shared_ptr<spdlog::logger> logger_;
    std::string_view device_;

    int fd_;
    int mux_;
  };
}

#endif // ICMP_TCPSOCKET_H
