#ifndef ICMP_COMMON_H
#define ICMP_COMMON_H


#include <netinet/if_ether.h>

namespace util
{
  const u_int8_t ALL_ZERO_MAC[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  const u_int8_t BCAST_MAC[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

  const u_int8_t PHANTOM_IP_ADDRESS[4] = { 0, 0, 0, 0 };
} // util


#endif //ICMP_COMMON_H
