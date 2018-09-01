#include "EthSender.h"

using namespace network;

EthSender::EthSender(const std::shared_ptr<cfg::Config> &config) {
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthSender");
  sock_ = std::make_unique<RawSocket>(config_->device());
}

template <typename T>
void EthSender::send(u_int16_t type, const bytes &dmac, const std::unique_ptr<Payload<T>> &pl)
{
  // ...
}
