#include "libutil/InternalErrorException.h"
#include "libutil/Common.h"
#include "Listener.h"

using namespace core;

const int Listener::N_EVENTS = 16;

Listener::Listener(std::shared_ptr<cfg::Config> config) {
  logger_ = spdlog::stdout_color_mt("Listener");
  config_ = config;
  sock_ = std::make_unique<nw::RawSocket>(config_->device());
  arp_ = std::make_unique<nw::Arp>(config_);

  setup_multiplexer();

  assign([this]{ this->wait(); });
}

Listener::~Listener() {
  stop();
}

void Listener::start() {
  run();
}

void Listener::stop() {
  terminate();
}

void Listener::setup_multiplexer() {
#if defined(__linux__)
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

void Listener::wait() {
#if defined(__linux__)
  int nfds = epoll_wait(mux_, events_, N_EVENTS, -1);
  if (nfds <= 0) {
    logger_->error("epoll_wait");
    return;
  }

  for (auto i = 0; i < nfds; i++) {
    if (events_[i].data.fd == sock_->fd()) {
      if (auto len = read(sock_->fd(), buf_, sizeof(buf_)) <= 0) {
        this->logger_->error("read");
      } else {
        struct ether_header *eh;
        u_int8_t *ptr = buf_;

        eh = (struct ether_header *) ptr;
        ptr += sizeof(struct ether_header);

        if (memcmp(eh->ether_dhost, util::BCAST_MAC, 6) != 0 && memcmp(eh->ether_dhost, config_->vmac()->as_str().c_str(), 6) != 0) {
          return;
        }

        if (ntohs(eh->ether_type) == ETHERTYPE_ARP) {
          arp_->recv(eh, ptr);
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
