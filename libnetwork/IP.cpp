#include "IP.h"

using namespace network;

IP::IP(const std::string& addr) {
  addr_.l = inet_addr(addr.c_str());
};

const u_int8_t * IP::as_byte() const {
  return addr_.c;
}

u_int32_t IP::as_numeric() {
  return addr_.l;
}
