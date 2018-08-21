#include <cstring>

#if defined(__linux__)
  #include <stdio.h>
  #include <ctype.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <string.h>
  #include <limits.h>
  #include <time.h>
  #include <sys/ioctl.h>
  #include <netpacket/packet.h>
  #include <netinet/ip.h>
  #include <netinet/ip_icmp.h>
  #include <netinet/if_ether.h>
  #include <linux/if.h>
  #include <arpa/inet.h>
#else
  // UNIX
  // ...
#endif

#include "libutil/InternalErrorException.h"

#include "RawSocket.h"

using namespace network;

const int RawSocket::NEVENTS = 16;

RawSocket::RawSocket(const std::string device) {
  device_ = device;
  logger_ = spdlog::stdout_color_mt("RawSocket");

  if (create_socket()) {
    throw util::InternalErrorException{ "Cannot create socket. (1)" };
  }

  if (setup_multiplexer()) {
    throw util::InternalErrorException{ "Cannot create socket. (2)" };
  }
}

RawSocket::~RawSocket() {
  logger_->info("デストラクタ！");
  close(fd_);
}

int RawSocket::create_socket() {
#if defined(__linux__)
  struct ifreq if_req;
  struct sockaddr_ll sa;

  if ((fd_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
    logger_->critical("socket");
    return -1;
  }

  strcpy(if_req.ifr_name, device_.c_str());
  if (ioctl(fd_, SIOCGIFINDEX, &if_req) < 0) {
    logger_->critical("ioctl (1)");
    close(fd_);
    return -1;
  }

  sa.sll_family = PF_PACKET;
  sa.sll_protocol = htons(ETH_P_ALL);
  sa.sll_ifindex = if_req.ifr_ifindex;
  if (bind(fd_, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    logger_->critical("bind");
    close(fd_);
    return -1;
  }

  if (ioctl(fd_, SIOCGIFFLAGS, &if_req) < 0) {
    logger_->critical("ioctl (2)");
    close(fd_);
    return -1;
  }

  if_req.ifr_flags = if_req.ifr_flags|IFF_PROMISC|IFF_UP;
  if (ioctl(fd_, SIOCSIFFLAGS, &if_req) < 0) {
    logger_->critical("ioctl (3)");
    close(fd_);
    return -1;
  }
#else
  // UNIX
  // ...
#endif

  return 0;
}

int RawSocket::setup_multiplexer() {
#if defined(__linux__)
  mux_ = epoll_create(NEVENTS);
  if (mux_ < 0) {
    logger_->error("epoll_create");
    return -1;
  }
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = fd_;
  if (epoll_ctl(mux_, EPOLL_CTL_ADD, fd_, &ev) != 0) {
    logger_->error("epoll_ctl");
    return -1;
  }
#else
  // UNIX
  // ...
#endif

  return 0;
}

void RawSocket::wait(std::function<void(const int fd)> fn) {
#if defined(__linux__)
  int nfds = epoll_wait(mux_, ev_ret_, NEVENTS, -1);
  if (nfds <= 0) {
    logger_->error("epoll_wait");
    return;
  }
  for (auto i = 0; i < nfds; i++) {
    if (ev_ret_[i].data.fd == fd_) {
      fn(fd_);
    }
  }
#else
  // UNIX
  // ...
#endif
}
