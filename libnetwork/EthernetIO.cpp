#include "libutil/Common.h"
#include "EthernetIO.h"

using namespace network;

EthernetIO::EthernetIO(std::shared_ptr<cfg::Config> config) {
  arp_ = std::make_unique<Arp>(config);
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthernetIO");
}

int EthernetIO::recv(u_int8_t *in_ptr, int in_len) {
  struct ether_header *eh;
  u_int8_t *ptr = in_ptr;
  int len = in_len;

  eh = (struct ether_header *) ptr;
  ptr += sizeof(struct ether_header);

  if (memcmp(eh->ether_dhost, util::BCAST_MAC, 6) != 0 && memcmp(eh->ether_dhost, config_->vmac()->as_str().c_str(), 6) != 0) {
    return -1;
  }

  if (ntohs(eh->ether_type) == ETHERTYPE_ARP) {
    arp_->recv(eh, ptr);
  } else if (ntohs(eh->ether_type) == ETHERTYPE_IP) {
    // ...
  }

  return 0;
}
