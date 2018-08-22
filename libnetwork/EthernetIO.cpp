#include "libutil/Common.h"
#include "EthernetIO.h"

using namespace network;

EthernetIO::EthernetIO() {
  logger_ = spdlog::stdout_color_mt("EthernetIO");
}
