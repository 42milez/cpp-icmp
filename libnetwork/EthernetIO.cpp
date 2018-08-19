#include "EthernetIO.h"

using namespace network;

const u_int8_t EthernetIO::BCAST_MAC[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

EthernetIO::EthernetIO(std::shared_ptr<cfg::Config> config) {
  config_ = config;
}

int EthernetIO::recv(u_int8_t *in_ptr, int in_len) {
  struct ether_header *eh;
  u_int8_t *ptr = in_ptr;
  int len = in_len;

  eh = (struct ether_header *) ptr;
  ptr += sizeof(struct ether_header);

  if (memcmp(eh->ether_dhost, BCAST_MAC, 6) != 0 && memcmp(eh->ether_dhost, config_->vmac()->mac()->c_str(), 6) != 0) {
    return -1;
  }

  if (ntohs(eh->ether_type) == ETHERTYPE_ARP) {
    arp_recv(eh, ptr);
  } else if (ntohs(eh->ether_type) == ETHERTYPE_IP) {
    // ...
  }

  return 0;
}

int EthernetIO::arp_recv(const ether_header *eh, u_int8_t *data) {
  struct ether_arp *arp;
  u_int8_t *ptr = data;

  arp = (struct ether_arp *) ptr;
  ptr += sizeof(struct ether_arp);

  if (ntohs(arp->arp_op) == ARPOP_REQUEST) {
    struct in_addr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
//      arp_add_table(arp, &addr);
//      arp_send(ARPOP_REPLY, eh, arp);
      logger_->info("request received");
    }
  }

  if (ntohs(arp->arp_op) == ARPOP_REPLY) {
    struct in_addr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (addr.s_addr == 0 || is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
//      arp_add_table(arp, &addr);
      logger_->info("reply received");
    }
  }

  return 0;
}

int EthernetIO::is_target_ip_addr(struct in_addr *addr) {
  if (config_->vip().s_addr == addr->s_addr) {
    return 1;
  }
  return 0;
}

