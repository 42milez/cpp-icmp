#include <cstddef>
#include <vector>

#ifdef __linux__
#include <arpa/inet.h>
#include <linux/if.h>
#include <netdb.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#else
// unix
// ...
#endif

#include "libutil/InternalErrorException.h"

#include "TcpSocket.h"

using namespace network;

TcpSocket::TcpSocket(std::string_view device)
{
  logger_ = spdlog::stdout_color_mt("TcpSocket");
  device_ = device;

  create_socket(8888);
}

TcpSocket::~TcpSocket()
{
  close(fd_);
}

void TcpSocket::create_socket(int port)
{
#ifdef __linux__
  struct addrinfo hints { AI_PASSIVE, AF_INET, SOCK_STREAM, 0, 0, nullptr, nullptr, nullptr };
  struct addrinfo *res0;

  int err;

  if ((err = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &res0)) != 0) {
    logger_->critical("getaddrinfo(): {0}", gai_strerror(err));
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  std::vector<char> nbuf;
  std::vector<char> sbuf;

  nbuf.reserve(NI_MAXHOST);
  sbuf.reserve(NI_MAXSERV);

  if ((err = getnameinfo(res0->ai_addr, res0->ai_addrlen,
                         nbuf.data(), nbuf.size(),
                         sbuf.data(), sbuf.size(),
                         NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
    freeaddrinfo(res0);
    logger_->critical("getnameinfo(): {0}", gai_strerror(err));
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  if ((fd_ = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol)) == -1) {
    logger_->critical("socket()");
    freeaddrinfo(res0);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  int opt { 1 };
  socklen_t opt_len { sizeof(opt) };

  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len) == -1) {
    logger_->critical("setsockopt()");
    close(fd_);
    freeaddrinfo(res0);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  if (bind(fd_, res0->ai_addr, res0->ai_addrlen) == -1) {
    logger_->critical("bind()");
    close(fd_);
    freeaddrinfo(res0);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  if (listen(fd_, SOMAXCONN) == -1) {
    logger_->critical("listen()");
    close(fd_);
    freeaddrinfo(res0);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  freeaddrinfo(res0);
#else
  // unix
  // ...
#endif
}

int TcpSocket::fd()
{
  return fd_;
}
