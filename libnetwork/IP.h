#ifndef ICMP_IP_H
#define ICMP_IP_H

#include <cstddef>
#include <string>
#include <vector>

namespace network
{
  using bytes = std::vector<std::byte>;

  class IP {
  public:
    IP(const std::string &addr);
    bytes as_byte() const;
    int as_decimal() const;
  private:
    std::string addr_;
  };
} // network

#endif // ICMP_IP_H
