#ifndef ICMP_IP_H
#define ICMP_IP_H


#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>

namespace network
{
  class IP {
  public:
    IP(const std::string& addr);
    const u_int8_t * as_byte() const;
    u_int32_t as_numeric();
  private:
    union {
      u_int32_t l;
      u_int8_t c[4];
    } addr_;
  };
} // network

#endif // ICMP_IP_H
