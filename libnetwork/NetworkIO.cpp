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

#include "NetworkIO.h"

using namespace network;

using std::cerr;
using std::cout;
using std::endl;
using std::to_string;

NetworkIO::NetworkIO(int port) {
  port_ = port;
  create_socket();
  create_multiplexer();
}

NetworkIO::~NetworkIO() {
  close(soc_);
}

void NetworkIO::create_raw_socket() {
  struct ifreq if_req;
  struct sockaddr_ll sa;

  if ((soc_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
    cerr << "socket" << endl;
    return;
  }

  strcpy(if_req.ifr_name, device);
  if (ioctl(soc_, SIOCGIFINDEX, &if_req) < 0) {
    cerr << "ioctl" << endl;
    return;
  }

  sa.sll_family = PF_PACKET;
  sa.sll_protocol = htons(ETH_P_ALL);
  sa.sll_ifindex = if_req.ifr_ifindex;
  if (bind(soc_, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    cerr << "bind" << endl;
    return;
  }

  if (ioctl(soc_, SIOCGFFLAGS, &if_req) < 0) {
    cerr << "ioctl" << endl;
    return;
  }

  if_req.ifr_flags = if_req.ifr_flags|IFF_PROMISC|IFF_UP;
  if (ioctl(soc_, SIOCSIFFLAGS, &if_req) < 0) {
    cerr << "ioctl" << endl;
    return;
  }
}

void NetworkIO::create_multiplexer() {
  mux_ = kqueue();

  if (mux_ == -1) {
    cout << "[ERROR] kqueue" << endl;
    return;
  }

  struct kevent event{ (unitptr_t) soc_, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, nullptr };

  kevent(mux_, &event, 1, nullptr, 0, nullptr);
}

void NetworkIO::wait_for_read(const function<void()> &fn) {
  struct kevent events[10];

  auto nfds = kevent(mux_, nullptr, 0, events, 10, nullptr);

  if (nfds == -1) {
    cout << "[ERROR] kevent" << endl;
    return;
  } else if (nfds == 0) {
    cout << "[ERROR] timeout" << endl;
    return
  } else {
    char *ptr;
    for (auto i = 0; i < nfds; i++) {
      auto soc = (int) events[i].ident;
      if (soc == soc_) {
        string buf(512, '\0');
        auto len = recv(soc, &buf[0], sizeof(buf), 0);
        buf.resize(len);
        fn();
      }
    }
  }
}
