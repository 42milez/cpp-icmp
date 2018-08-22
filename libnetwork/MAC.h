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
    const std::vector<u_int8_t>& as_hex();
    const std::string& as_str();
  private:
    std::vector<std::string> mac_;
    std::vector<u_int8_t> mac_hex_;
    std::string mac_str_;
  };
} // network


#endif // ICMP_MAC_H
