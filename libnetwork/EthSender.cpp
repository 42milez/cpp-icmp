#include <unistd.h>

#include "Arp.h"
#include "EthSender.h"

using namespace network;

EthSender::EthSender(const std::shared_ptr<cfg::Config> &config) {
  config_ = config;
  logger_ = spdlog::stdout_color_mt("EthSender");
  sock_ = std::make_unique<RawSocket>(config_->device(), "RawSocket(EthSender)");
}

template <typename T>
void EthSender::send(u_int16_t type, const bytes &dmac, const std::unique_ptr<Payload<T>> &pl) {
  if (pl->len > ETHERMTU) {
    return;
  }

  u_int8_t *buf_ptr, buf[sizeof(struct ether_header) + ETHERMTU];

  buf_ptr = buf;

  auto eh = reinterpret_cast<struct ether_header *>(buf);
  memcpy(eh->ether_dhost, dmac.data(), 6);
  memcpy(eh->ether_shost, config_->vmac()->as_hex().data(), 6);
  eh->ether_type = htons(type);
  buf_ptr += sizeof(struct ether_header);

  memcpy(buf_ptr, pl->data.get(), pl->len);
  buf_ptr += pl->len;

  // @todo: Use ETH_ZLEN
  if ((buf_ptr - buf) < 60) {
    auto padlen = 60 - (buf_ptr - buf);
    memset(buf_ptr, 0, padlen);
    buf_ptr += padlen;
  }

  write(sock_->fd(), buf, buf_ptr - buf);
}

// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
// http://www.cplusplus.com/articles/1C75fSEw/
// https://qiita.com/i153/items/38f9688a9c80b2cb7da7
template void EthSender::send<EthArp>(u_int16_t type, const bytes &dmac, const std::unique_ptr<Payload<EthArp>> &pl);
