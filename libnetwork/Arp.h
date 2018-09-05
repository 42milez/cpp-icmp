#ifndef ICMP_ARP_H
#define ICMP_ARP_H


#include <vector>

#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include <spdlog/spdlog.h>

#include "libconfig/Config.h"
#include "EthSender.h"

namespace network
{
  using bytes = std::vector<std::byte>;
  using EthArp = struct ether_arp;
  using EthHeader = struct ether_header;

  using ArpEntry = struct {
    time_t timestamp;
    u_int8_t mac[6];
    struct in_addr ipaddr;
  };

  class Arp {
  public:
    Arp(const std::shared_ptr<config::Config> &config);
    // https://stackoverflow.com/questions/17156282/passing-a-stdarray-of-unknown-size-to-a-function
    void recv(const bytes &buf);
    void gratuitous(const bytes &buf);
    void request(const bytes &buf);
  private:
    void add_table();
    std::unique_ptr<Payload<EthArp>> build_payload(u_int16_t op, const bytes &buf);
    void reply(const bytes &buf);
    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<config::Config> config_;
    std::unique_ptr<EthSender> sender_;
    ArpEntry arp_entries_;
  };
} // network


#endif // ICMP_ARP_H
