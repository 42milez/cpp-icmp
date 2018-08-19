#include "EthernetIO.h"

using namespace network;

const u_int8_t EthernetIO::BCAST_MAC[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

EthernetIO::EthernetIO(std::shared_ptr<cfg::Config> config) {
  arp_ = std::make_unique<Arp>();
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthernetIO");
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
    arp_->recv(eh, ptr);
  } else if (ntohs(eh->ether_type) == ETHERTYPE_IP) {
    // ...
  }

  return 0;
}

int EthernetIO::is_target_ip_addr(const struct in_addr *addr) {
  if (config_->vip().s_addr == addr->s_addr) {
    return 1;
  }
  return 0;
}

int EthernetIO::is_same_subnet(const struct in_addr *addr) {
  if ((addr->s_addr & config_->vmask().s_addr)==(config_->vip().s_addr & config_->vmask().s_addr)) {
    return 1;
  }
  else {
    return 0;
  }
}
