#ifndef ICMP_LISTENER_H
#define ICMP_LISTENER_H


#include <memory>
#include <string>

#if defined(__linux__)
  #include <sys/epoll.h>
#else
  // UNIX
  // ...
#endif

#include <spdlog/spdlog.h>

#include "libnetwork/Arp.h"
#include "libnetwork/RawSocket.h"
#include "Worker.h"

namespace core
{
  namespace cfg = config;
  namespace nw = network;

#if defined(__linux__)
  using EpEvt = struct epoll_event;
#endif

  class Listener : public Worker {
  public:
    explicit Listener(std::shared_ptr<cfg::Config> config);
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    void setup_multiplexer();
    void wait();

    static const int N_EVENTS;

    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<cfg::Config> config_;
    std::unique_ptr<nw::RawSocket> sock_;
    std::unique_ptr<nw::Arp> arp_;
    u_int8_t buf_[2048];
    int mux_;

#if defined(__linux__)
    EpEvt events_[16];
#else
    // UNIX
    // ...
#endif
  };
} // core


#endif // ICMP_LISTENER_H
