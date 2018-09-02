#include "EthSender.h"

using namespace network;

EthSender::EthSender(const std::shared_ptr<cfg::Config> &config) {
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthSender");
  sock_ = std::make_unique<RawSocket>(config_->device());
}
