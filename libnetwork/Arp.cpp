#include "Arp.h"

using namespace network;

namespace cfg = config;

const int N_ARP_TABLES = 16;

Arp::Arp(std::shared_ptr<cfg::Config> config) {
  logger_ = spdlog::stdout_color_mt("Arp");
  config_ = config;
}

void Arp::add_table() {
  // ...
}

int Arp::recv(ether_header *eh, const u_int8_t *buf) {
  auto arp = (EthArp *) buf;

  if (ntohs(arp->arp_op) == ARPOP_REQUEST) {
    logger_->info("request received");
    cfg::InAddr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (config_->is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
      //add_table(arp, &addr);
      send(ARPOP_REPLY, eh, arp, config_->vmac()->as_hex());
    }
  }

  if (ntohs(arp->arp_op) == ARPOP_REPLY) {
    logger_->info("reply received");
    cfg::InAddr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (addr.s_addr == 0 || config_->is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
      //add_table(arp, &addr);
    }
  }

  return 0;
}

void Arp::send(int fd, const EthHeader *eh, const EthArp *ea, const std::vector<u_int8_t>& vmac) {
  // ...
}
