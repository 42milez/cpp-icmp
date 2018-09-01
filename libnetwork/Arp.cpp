#include <cstring>

#include "libutil/Common.hpp"
#include "Arp.h"
#include "EthSender.h"

using namespace network;

namespace cfg = config;

const int N_ARP_TABLES = 16;

std::unique_ptr<Payload<EthArp>> build_payload(const u_char *sha, const bytes &spa, const bytes &tha, const u_char *tpa);

Arp::Arp(std::shared_ptr<cfg::Config> config) {
  logger_ = spdlog::stdout_color_mt("Arp");
  config_ = config;
  sender_ = std::make_unique<EthSender>(config_);
}

void Arp::add_table() {
  // ...
}

void Arp::recv(ether_header *eh, const u_int8_t *buf) {
  auto arp = (EthArp *) buf;

  if (ntohs(arp->arp_op) == ARPOP_REQUEST) {
    logger_->info("request received");
    struct in_addr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (config_->is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
      //add_table(arp, &addr);
      //reply();
    }
  }

  if (ntohs(arp->arp_op) == ARPOP_REPLY) {
    logger_->info("reply received");
    struct in_addr addr;
    addr.s_addr = (arp->arp_tpa[3] << 24) | (arp->arp_tpa[2] << 16) | (arp->arp_tpa[1] << 8) | (arp->arp_tpa[0]);
    if (addr.s_addr == 0 || config_->is_target_ip_addr(&addr)) {
      addr.s_addr = (arp->arp_spa[3] << 24) | (arp->arp_spa[2] << 16) | (arp->arp_spa[1] << 8) | (arp->arp_spa[0]);
      //add_table(arp, &addr);
    }
  }
}

//  IPアドレスの重複チェックを行う
// --------------------------------------------------
//  - 宛先MACとIPには送信元ホストのものを指定
//  - 受信側は以下を条件に返答する
//    - 宛先IPアドレスがホストのIPアドレスと一致する
//    - 宛先MACアドレスが0ではない
void Arp::gratuitous() {
  auto payload = build_payload(config_->vmac()->as_hex().data(), util::PHANTOM_IP_ADDRESS, util::ALL_ZERO_MAC, config_->vip()->as_byte());
}

//  IPアドレスからMACアドレスを引く
// --------------------------------------------------
//  - 宛先MACには0を指定
//  - 宛先IPには通信相手のIPアドレス指定
void Arp::request(const IP& tpa) {
  auto payload = build_payload(config_->vmac()->as_hex().data(), config_->vip()->as_byte(), util::ALL_ZERO_MAC, tpa.as_byte());
  sender_->send(ETHERTYPE_ARP, util::BCAST_MAC, payload);
}

void Arp::reply(u_int8_t dmac, u_int8_t daddr) {

}

std::unique_ptr<Payload<EthArp>> build_payload(const u_char *sha, const bytes &spa, const bytes &tha, const u_char *tpa) {
  std::unique_ptr<Payload<EthArp>> payload = std::make_unique<Payload<EthArp>>();
  std::unique_ptr<EthArp> arp = std::make_unique<EthArp>();

  arp->arp_hrd = htons(ARPHRD_ETHER);
  arp->arp_pro = htons(ETHERTYPE_IP);
  arp->arp_hln = 6;
  arp->arp_pln = 4;
  arp->arp_op = htons(ARPOP_REQUEST);

  std::memcpy(arp->arp_sha, sha, 6);
  std::memcpy(arp->arp_spa, spa.data(), 6);

  std::memcpy(arp->arp_tha, tha.data(), 4);
  std::memcpy(arp->arp_tpa, tpa, 4);

  payload->data = std::move(arp);

  return std::move(payload);
}
