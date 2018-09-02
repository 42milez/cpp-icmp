#include <arpa/inet.h>
#include <cstdlib>
#include <netinet/in.h>

#include "IP.h"

using namespace network;

IP::IP(const std::string& addr) {
  union {
    struct in_addr l;
    u_int8_t c[4];
  } tmp { 0 };

  inet_aton(addr.c_str(), &tmp.l);

  addr_.emplace_back(static_cast<std::byte>(tmp.c[0]));
  addr_.emplace_back(static_cast<std::byte>(tmp.c[1]));
  addr_.emplace_back(static_cast<std::byte>(tmp.c[2]));
  addr_.emplace_back(static_cast<std::byte>(tmp.c[3]));
};

const bytes & IP::as_byte() const {
  return addr_;
}

int IP::as_decimal() {
  struct in_addr tmp { 0 };
  inet_aton(reinterpret_cast<char *>(addr_.data()), &tmp);
  return tmp.s_addr;
}
