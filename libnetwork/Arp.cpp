#include "Arp.h"

using namespace network;

namespace cfg = config;

const int Arp::N_ARP_TABLES = 16;

Arp::Arp(std::shared_ptr<cfg::Config> config) {
  config_ = config;
  logger_ = spdlog::stdout_color_mt("Arp");
}

void Arp::add_table() {

}

int Arp::recv(const ether_header *eh, u_int8_t *data) {
  struct ether_arp *arp;
  u_int8_t *ptr = data;

  arp = (struct ether_arp *) ptr;
  ptr += sizeof(struct ether_arp);

  if (ntohs(arp->arp_op) == ARPOP_REQUEST) {
    logger_->info("request received");
    struct in_addr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (config_->is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
      //add_table(arp, &addr);
      send(ARPOP_REPLY, eh, arp, config_->vmac()->as_hex());
    }
  }

  if (ntohs(arp->arp_op) == ARPOP_REPLY) {
    logger_->info("reply received");
    struct in_addr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (addr.s_addr == 0 || config_->is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
      //add_table(arp, &addr);
    }
  }

  return 0;
}

void Arp::send(int fd, const EthHeader *eh, const EthArp *ea, const std::vector<u_int8_t>& vmac) {

}
