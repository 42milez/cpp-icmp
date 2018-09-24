#include <arpa/inet.h>
#include <cstdlib>
#include <netinet/in.h>

#include "IP.h"

using namespace network;

IP::IP(const std::string& addr) {
  addr_ = addr;
};

bytes IP::as_byte() const {
  bytes addr;

  union {
    struct in_addr l;
    u_int8_t c[4];
  } tmp { 0 };

  inet_aton(addr_.c_str(), &tmp.l);

  addr.emplace_back(static_cast<std::byte>(tmp.c[0]));
  addr.emplace_back(static_cast<std::byte>(tmp.c[1]));
  addr.emplace_back(static_cast<std::byte>(tmp.c[2]));
  addr.emplace_back(static_cast<std::byte>(tmp.c[3]));

  return addr;
}

int IP::as_decimal() const {
  bytes addr = as_byte();
  struct in_addr tmp { 0 };

  inet_aton(reinterpret_cast<char *>(addr.data()), &tmp);

  return tmp.s_addr;
}
