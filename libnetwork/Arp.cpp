#include "Arp.h"

using namespace network;

Arp::Arp() {
  logger_ = spdlog::stdout_color_mt("Arp");
}

int Arp::recv(const ether_header *eh, u_int8_t *data) {
  struct ether_arp *arp;
  u_int8_t *ptr = data;

  arp = (struct ether_arp *) ptr;
  ptr += sizeof(struct ether_arp);

  if (ntohs(arp->arp_op) == ARPOP_REQUEST) {
    //    struct in_addr addr;
    //    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    //    if (is_target_ip_addr(&addr)) {
    //      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
    //      arp_add_table(arp, &addr);
    //      arp_send(ARPOP_REPLY, eh, arp);
    //    }
    logger_->info("request received");
  }

  if (ntohs(arp->arp_op) == ARPOP_REPLY) {
    //    struct in_addr addr;
    //    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    //    if (addr.s_addr == 0 || is_target_ip_addr(&addr)) {
    //      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
    //      arp_add_table(arp, &addr);
    //    }
    logger_->info("reply received");
  }

  return 0;
}
