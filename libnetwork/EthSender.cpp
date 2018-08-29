#include "libutil/Common.hpp"
#include "EthSender.h"

using namespace network;

EthSender::EthSender() {
  logger_ = spdlog::stdout_color_mt("EthSender");
  sock_ = std::make_unique<nw::RawSocket>(config_->device());
}

void EthSender::send(void send(const u_int16_t type, std::unique_ptr<MAC> &smac, std::unique_ptr<MAC> &dmac, const Payload &pl))
{
  // ...
}
