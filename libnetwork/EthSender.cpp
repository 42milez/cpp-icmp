#include "Arp.h"
#include "EthSender.h"

using namespace network;

EthSender::EthSender(const std::shared_ptr<cfg::Config> &config) {
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthSender");
  sock_ = std::make_unique<RawSocket>(config_->device());
}

template <typename T>
void EthSender::send(u_int16_t type, const bytes &dmac, const std::unique_ptr<Payload<T>> &pl) {

}

// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
// http://www.cplusplus.com/articles/1C75fSEw/
// https://qiita.com/i153/items/38f9688a9c80b2cb7da7
template void EthSender::send<EthArp>(u_int16_t type, const bytes &dmac, const std::unique_ptr<Payload<EthArp>> &pl);
