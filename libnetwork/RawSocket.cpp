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

#include "RawSocket.h"

using namespace network;

using std::cerr;
using std::cout;
using std::endl;
using std::to_string;

RawSocket::RawSocket(int port) {
  create_socket();
  setup_multiplexer();
}

RawSocket::~RawSocket() {

}

void RawSocket::create_socket() {
#if defined(__linux__)
  struct ifreq if_req;
  struct sockaddr_ll sa;

  if ((fd_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
    cerr << "socket" << endl;
    return;
  }

  strcpy(if_req.ifr_name, device);
  if (ioctl(fd_, SIOCGIFINDEX, &if_req) < 0) {
    cerr << "ioctl" << endl;
    return;
  }

  sa.sll_family = PF_PACKET;
  sa.sll_protocol = htons(ETH_P_ALL);
  sa.sll_ifindex = if_req.ifr_ifindex;
  if (bind(fd_, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    cerr << "bind" << endl;
    return;
  }

  if (ioctl(fd_, SIOCGFFLAGS, &if_req) < 0) {
    cerr << "ioctl" << endl;
    return;
  }

  if_req.ifr_flags = if_req.ifr_flags|IFF_PROMISC|IFF_UP;
  if (ioctl(fd_, SIOCSIFFLAGS, &if_req) < 0) {
    cerr << "ioctl" << endl;
    return;
  }
#else
  // UNIX
  // ...
#endif
}

void RawSocket::setup_multiplexer() {
#if defined(__linux__)
  mux_ = epoll_create(NEVENTS);
  if (epfd < 0) {
    cerr << "epoll_create" << endl;
    return;
  }
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = fd_;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd_, &ev) != 0) {
    cerr << "epoll_ctl" << endl;
    return;
  }
#else
  // UNIX
  // ...
#endif
}

void RawSocket::wait() {
#if defined(__linux__)
  int nfds = epoll_wait(mux_, ev_ret, NEVENTS, -1);
  if (nfds <= 0) {
    cerr << "epoll_wait" << endl;
    return;
  }
  for (auto i = 0; i < nfds; i++) {
    if (ev_ret_[i].data.fd == fd_) {
      // ...
    }
  }
#else
  // UNIX
  // ...
#endif
}
