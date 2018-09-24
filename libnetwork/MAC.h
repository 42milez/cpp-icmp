#ifndef ICMP_MAC_H
#define ICMP_MAC_H


#include <memory>
#include <string_view>
#include <vector>

namespace network
{
  using sv = std::string_view;

  class MAC {
  public:
    explicit MAC(const std::string& mac);
    std::vector<long> as_hex();
    sv::const_pointer as_str();
  private:
    static inline const char* MAC_ADDRESS_DELIMITER = ":";
    static inline const char* MAC_ADDRESS_PATTERN = "[0-9a-fA-F]{2}:"
                                                    "[0-9a-fA-F]{2}:"
                                                    "[0-9a-fA-F]{2}:"
                                                    "[0-9a-fA-F]{2}:"
                                                    "[0-9a-fA-F]{2}:"
                                                    "[0-9a-fA-F]{2}";
    sv mac_;
  };
} // network


#endif // ICMP_MAC_H
