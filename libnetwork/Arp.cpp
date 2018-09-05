#include <cstring>

#include "libutil/Common.h"
#include "Arp.h"
#include "EthSender.h"

using namespace network;

const int N_ARP_TABLES = 16;

Arp::Arp(const std::shared_ptr<config::Config> &config) {
  logger_ = spdlog::stdout_color_mt("Arp");
  config_ = config;
  sender_ = std::make_unique<EthSender>(config_);
}

void Arp::add_table() {
  // ...
}

void Arp::recv(const bytes &buf) {
  auto eth = (EthHeader *) buf.data();
  auto arp = (EthArp *) util::extract_eth_payload(buf);

  auto tpa = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
  auto spa = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);

  if (ntohs(arp->arp_op) == ARPOP_REQUEST) {
    logger_->info("request received");
    if (config_->is_target_ip_addr(tpa)) {
      //add_table(arp, &addr);
      reply(buf);
    }
    return;
  }

  if (ntohs(arp->arp_op) == ARPOP_REPLY) {
    logger_->info("reply received");
    if (tpa == 0 || config_->is_target_ip_addr(tpa)) {
      //add_table(arp, &addr);
    }
    return;
  }
}

//  IPアドレスの重複チェックを行う
// --------------------------------------------------
//  - 宛先MACとIPには送信元ホストのものを指定
//  - 受信側は以下を条件に返答する
//    - 宛先IPアドレスがホストのIPアドレスと一致する
//    - 宛先MACアドレスが0ではない
void Arp::gratuitous(const bytes &buf) {
  auto payload = build_payload(ARPOP_REQUEST, buf);
}

//  IPアドレスからMACアドレスを引く
// --------------------------------------------------
//  - 宛先MACには0を指定
//  - 宛先IPには通信相手のIPアドレス指定
void Arp::request(const bytes &buf) {
  std::unique_ptr<Payload<EthArp>> payload = build_payload(ARPOP_REQUEST, buf);
  sender_->send(ETHERTYPE_ARP, util::BCAST_MAC, payload);
}

void Arp::reply(const bytes &buf) {
  std::unique_ptr<Payload<EthArp>> payload = build_payload(ARPOP_REPLY, buf);
  sender_->send(ETHERTYPE_ARP, util::BCAST_MAC, payload);
}

std::unique_ptr<Payload<EthArp>> Arp::build_payload(u_int16_t op, const bytes &buf) {
  std::unique_ptr<Payload<EthArp>> payload = std::make_unique<Payload<EthArp>>();
  std::unique_ptr<EthArp> arp = std::make_unique<EthArp>();

  arp->arp_hrd = htons(ARPHRD_ETHER);
  arp->arp_pro = htons(ETHERTYPE_IP);
  arp->arp_hln = 6;
  arp->arp_pln = 4;
  arp->arp_op = htons(op);

  std::memcpy(arp->arp_sha, config_->vmac()->as_hex().data(), 6);
  std::memcpy(arp->arp_spa, config_->vip()->as_byte().data(), 4);

  std::memcpy(arp->arp_tha, arp->arp_tha, 6);
  std::memcpy(arp->arp_tpa, arp->arp_tpa, 4);

  payload->data = std::move(arp);

  return payload;
}
