#ifndef ICMP_LISTENER_H
#define ICMP_LISTENER_H


#include <array>
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
    explicit Listener(std::shared_ptr<cfg::Config> &config);
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    static const size_t N_EVENTS = 16;
    static const size_t SIZE_BUFFER = 2048;

    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<cfg::Config> config_;
    std::unique_ptr<nw::RawSocket> sock_;
    std::unique_ptr<nw::Arp> arp_;
    int mux_;

    void setup_multiplexer();
    void wait();
#if defined(__linux__)
    std::array<EpEvt,N_EVENTS> events_;
#else
    // UNIX
    // ...
#endif
  };
} // core


#endif // ICMP_LISTENER_H
