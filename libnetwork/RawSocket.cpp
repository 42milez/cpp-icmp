#include <cstring>

#ifdef __linux__
  #include <arpa/inet.h>
  #include <linux/if.h>
  #include <netinet/if_ether.h>
  #include <netinet/ip.h>
  #include <netinet/ip_icmp.h>
  #include <netpacket/packet.h>
  #include <sys/ioctl.h>
  #include <ctype.h>
  #include <limits.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  #include <unistd.h>
#else
  // UNIX
  // ...
#endif

#include "libutil/InternalErrorException.h"

#include "RawSocket.h"

using namespace network;

RawSocket::RawSocket(const std::string& device) {
  device_ = device;
  create_socket();
}

RawSocket::~RawSocket() {
  close(fd_);
}

void RawSocket::create_socket() {
#ifdef __linux__
  struct ifreq if_req;
  struct sockaddr_ll sa;

  if ((fd_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  strcpy(if_req.ifr_name, device_.c_str());
  if (ioctl(fd_, SIOCGIFINDEX, &if_req) < 0) {
    close(fd_);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  sa.sll_family = PF_PACKET;
  sa.sll_protocol = htons(ETH_P_ALL);
  sa.sll_ifindex = if_req.ifr_ifindex;
  if (bind(fd_, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    close(fd_);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  if (ioctl(fd_, SIOCGIFFLAGS, &if_req) < 0) {
    close(fd_);
    throw util::InternalErrorException{ "Cannot create socket." };
  }

  if_req.ifr_flags = if_req.ifr_flags|IFF_PROMISC|IFF_UP;
  if (ioctl(fd_, SIOCSIFFLAGS, &if_req) < 0) {
    close(fd_);
    throw util::InternalErrorException{ "Cannot create socket." };
  }
#else
  // UNIX
  // ...
#endif
}

int RawSocket::fd() {
  return fd_;
}
