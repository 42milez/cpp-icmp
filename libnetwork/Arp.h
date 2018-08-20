#ifndef ICMP_ARP_H
#define ICMP_ARP_H


#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include <spdlog/spdlog.h>

namespace network
{
  class Arp {
  public:
    Arp();
    int recv(const ether_header *eh, u_int8_t *data);
  private:
    std::shared_ptr<spdlog::logger> logger_;
  };
} // network


#endif // ICMP_ARP_H
