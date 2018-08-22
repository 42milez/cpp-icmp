#ifndef ICMP_ETHERNETIO_H
#define ICMP_ETHERNETIO_H


#include <memory>
#include <string>

#include <spdlog/spdlog.h>

#include "libconfig/Config.h"
#include "Arp.h"

namespace network
{
  namespace cfg = config;

  class EthernetIO {
  public:
    EthernetIO();
  private:
    std::shared_ptr<spdlog::logger> logger_;
  };
} // network


#endif // ICMP_ETHERNETIO_H
