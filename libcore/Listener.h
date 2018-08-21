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

#include "libnetwork/EthernetIO.h"
#include "libnetwork/RawSocket.h"
#include "libnetwork/Arp.h"
#include "Worker.h"

namespace core
{
  namespace cfg = config;
  namespace nw = network;

  class Listener : public Worker {
  public:
    explicit Listener(std::shared_ptr<cfg::Config> config);
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    int setup_multiplexer();
    void wait();
#if defined(__linux__)
    struct epoll_event ev_ret_[16];
#else
    // UNIX
    // ...
#endif
    std::unique_ptr<nw::Arp> arp_;
    u_int8_t buf_[2048];
    std::shared_ptr<cfg::Config> config_;
    std::unique_ptr<nw::EthernetIO> eth_;
    int mux_;
    std::unique_ptr<nw::RawSocket> sock_;
    std::shared_ptr<spdlog::logger> logger_;
  };
} // core


#endif // ICMP_LISTENER_H
