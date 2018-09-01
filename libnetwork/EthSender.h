#ifndef ICMP_ETHSENDER_H
#define ICMP_ETHSENDER_H


#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "libconfig/Config.h"
#include "RawSocket.h"

namespace network
{
  namespace cfg = config;

  using bytes = std::vector<std::byte>;

  template <typename T>
  struct Payload {
    std::unique_ptr<T> data;
    int len;
  };

  class EthSender {
  public:
    EthSender(const std::shared_ptr<cfg::Config> &config);
    template <typename T>
    void send(u_int16_t type, const bytes &dmac, const std::unique_ptr<Payload<T>> &pl);
  private:
    std::shared_ptr<cfg::Config> config_;
    std::shared_ptr<spdlog::logger> logger_;
    std::unique_ptr<RawSocket> sock_;
  };
} // network


#endif // ICMP_ETHSENDER_H
