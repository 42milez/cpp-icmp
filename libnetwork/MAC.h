#ifndef ICMP_MAC_H
#define ICMP_MAC_H


#include <memory>
#include <string>
#include <vector>

namespace network
{
  class MAC {
  public:
    explicit MAC(const std::string& mac);
    std::vector<long> as_hex();
    const std::string& as_str();
  private:
    std::string mac_;
  };
} // network


#endif // ICMP_MAC_H
