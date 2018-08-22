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

  using std::string;

  class EthernetIO {
  public:
    EthernetIO(std::shared_ptr<cfg::Config> config);
  private:
    std::shared_ptr<cfg::Config> config_;
    std::shared_ptr<spdlog::logger> logger_;
  };
} // network


#endif // ICMP_ETHERNETIO_H
