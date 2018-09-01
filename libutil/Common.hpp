#ifndef ICMP_COMMON_H
#define ICMP_COMMON_H


#include <cstddef>
#include <vector>

#include <netinet/if_ether.h>

namespace util
{
  const std::vector<std::byte> ALL_ZERO_MAC = { std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00) };
  const std::vector<std::byte> BCAST_MAC { std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF) };
  const std::vector<std::byte> PHANTOM_IP_ADDRESS = { std::byte(0), std::byte(0), std::byte(0), std::byte(0) };
} // util


#endif //ICMP_COMMON_H
