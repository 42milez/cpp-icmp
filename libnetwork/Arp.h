#ifndef ICMP_ARP_H
#define ICMP_ARP_H


#include <vector>

#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include <spdlog/spdlog.h>

#include "libconfig/Config.h"

namespace network
{
  namespace cfg = config;

  using EthArp = struct ether_arp;
  using EthHeader = struct ether_header;

  using ArpEntry = struct {
    time_t timestamp;
    u_int8_t mac[6];
    struct in_addr ipaddr;
  };

  class Arp {
  public:
    Arp(std::shared_ptr<cfg::Config> config);
    // https://stackoverflow.com/questions/17156282/passing-a-stdarray-of-unknown-size-to-a-function
    int recv(EthHeader *eh, const u_int8_t *buf);
  private:
    void add_table();
    void send(int fd, const EthHeader *eh, const EthArp *ea, const std::vector<u_int8_t>& vmac);
    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<cfg::Config> config_;
    ArpEntry arp_entries_;
  };
} // network


#endif // ICMP_ARP_H
