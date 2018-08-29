#ifndef ICMP_ETHSENDER_H
#define ICMP_ETHSENDER_H


#include <memory>
#include <string>

#include <spdlog/spdlog.h>

#include "libconfig/Config.h"
#include "Arp.h"

namespace network
{
  namespace cfg = config;

  using Peyload = struct {
    u_int8_t *data,
    int len
  };

  class EthSender {
  public:
    EthSender();
    void send(const u_int16_t type, u_int8_t smac[6], u_int8_t dmac[6], const Payload &pl);
  private:
    std::shared_ptr<spdlog::logger> logger_;
    std::unique_ptr<nw::RawSocket> sock_;
  };
} // network


#endif // ICMP_ETHSENDER_H
