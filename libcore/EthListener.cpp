#include "libutil/InternalErrorException.h"
#include "libutil/Common.h"
#include "EthListener.h"

using namespace core;

using bytes = std::vector<std::byte>;

EthListener::EthListener(std::shared_ptr<cfg::Config> &config) {
  logger_ = spdlog::stdout_color_mt("EthListener");
  config_ = config;
  sock_ = std::make_unique<nw::RawSocket>(config_->device());
  arp_ = std::make_unique<nw::Arp>(config_);

  setup_multiplexer();

  assign([this]{ this->wait(); });
}

EthListener::~EthListener() {
  stop();
}

void EthListener::start() {
  run();
}

void EthListener::stop() {
  terminate();
}

void EthListener::setup_multiplexer() {
#ifdef __linux__
  mux_ = epoll_create(N_EVENTS);
  if (mux_ < 0) {
    logger_->error("epoll_create");
    throw util::InternalErrorException{ "Cannot create event poll." };
  }

  EpEvt ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = sock_->fd();
  if (epoll_ctl(mux_, EPOLL_CTL_ADD, sock_->fd(), &ev) != 0) {
    logger_->error("epoll_ctl");
    throw util::InternalErrorException{ "Cannot create event poll." };
  }
#else
  // UNIX
  // ...
#endif
}

void EthListener::wait() {
#ifdef __linux__
  int nfds = epoll_wait(mux_, events_.data(), N_EVENTS, -1);
  if (nfds <= 0) {
    logger_->error("epoll_wait");
    return;
  }

  for (auto i = 0; i < nfds; i++) {
    if (events_[i].data.fd == sock_->fd()) {
      bytes buf = bytes(SIZE_BUFFER);
      if (auto len = read(sock_->fd(), buf.data(), SIZE_BUFFER) <= 0) {
        this->logger_->error("read");
      } else {
        auto eh = (nw::EthHeader *) buf.data();
        if (memcmp(eh->ether_dhost, util::BCAST_MAC.data(), 6) != 0 && memcmp(eh->ether_dhost, config_->vmac()->as_str(), 6) != 0) {
          return;
        }

        if (ntohs(eh->ether_type) == ETHERTYPE_ARP) {
          arp_->recv(buf);
        } else if (ntohs(eh->ether_type) == ETHERTYPE_IP) {
          // icmp
          // ...
        }
      }
    }
  }
#else
  // UNIX
  // ...
#endif
}
