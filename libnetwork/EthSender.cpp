#include "libutil/Common.hpp"
#include "EthSender.h"

using namespace network;

EthSender::EthSender() {
  logger_ = spdlog::stdout_color_mt("EthSender");
}
