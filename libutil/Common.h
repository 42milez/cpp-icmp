#ifndef ICMP_COMMON_H
#define ICMP_COMMON_H


#include <cstddef>
#include <vector>

#include <netinet/if_ether.h>

namespace util
{
  using bytes = std::vector<std::byte>;

  const bytes ALL_ZERO_MAC = { std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00) };
  const bytes BCAST_MAC { std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF) };
  const bytes PHANTOM_IP_ADDRESS = { std::byte(0), std::byte(0), std::byte(0), std::byte(0) };

  inline const std::byte * extract_eth_payload(const bytes &buf) {
    return buf.data() + sizeof(struct ether_header);
  }
} // util


#endif //ICMP_COMMON_H
