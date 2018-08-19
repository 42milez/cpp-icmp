#ifndef ICMP_ETHERNETIO_H
#define ICMP_ETHERNETIO_H


#include <memory>
#include <string>

#include <netinet/if_ether.h>

#include <spdlog/spdlog.h>

#include "libconfig/Config.h"
#include "Arp.h"

namespace network
{
  namespace cfg = config;

  using std::string;

  class EthernetIO {
  public:
    EthernetIO(std::shared_ptr<cfg::Config> config);
    int recv(u_int8_t* in_ptr, int in_len);
  private:
    int is_target_ip_addr(const struct in_addr *addr);
    int is_same_subnet(const struct in_addr *addr);
    static const uint8_t BCAST_MAC[];
    std::unique_ptr<Arp> arp_;
    std::shared_ptr<cfg::Config> config_;
    std::shared_ptr<spdlog::logger> logger_;
  };
} // network


#endif // ICMP_ETHERNETIO_H
