#include "libutil/Common.h"
#include "EthernetIO.h"

using namespace network;

EthernetIO::EthernetIO(std::shared_ptr<cfg::Config> config) {
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthernetIO");
}
