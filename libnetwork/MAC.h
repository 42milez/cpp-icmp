#ifndef ICMP_MAC_H
#define ICMP_MAC_H


#include <string>
#include <vector>

namespace network
{
  class MAC {
  public:
    explicit MAC(std::string mac);
  private:
    static const std::string MAC_ADDRESS_DELIMITER;
    static const std::string MAC_ADDRESS_PATTERN;

    std::vector<std::string> mac_;
  };
} // network


#endif // ICMP_MAC_H
