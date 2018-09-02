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
    IP(const std::string& addr);
    const bytes & as_byte() const;
    int as_decimal();
  private:
    bytes addr_;
  };
} // network

#endif // ICMP_IP_H
