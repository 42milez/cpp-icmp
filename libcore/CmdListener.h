#ifndef ICMP_CMDLISTENER_H
#define ICMP_CMDLISTENER_H

#include <cstddef>
#include <string_view>

#include <spdlog/spdlog.h>

#include "libnetwork/TcpSocket.h"
#include "Worker.h"

namespace core
{
  namespace nw = network;

  using bytes = std::vector<std::byte>;

#ifdef __linux__
  using EpEvt = struct epoll_event;
#endif

  class CmdListener : public Worker {
  public:
    CmdListener(std::string_view device);
    ~CmdListener();
    void start() override;
    void stop() override;
  private:
    void setup_multiplexer();
    void wait();
    void parse(const std::string &cmd);
    void exec_arp(const std::string &option);

    static const size_t N_EVENTS = 16;
    static const size_t SIZE_BUFFER = 2048;

    std::shared_ptr<spdlog::logger> logger_;
    std::unique_ptr<nw::TcpSocket> sock_;

    int mux_;
#ifdef __linux__
    std::array<EpEvt, N_EVENTS> events_;
#else
    // Unix
    // ...
#endif
  };
} // core

#endif // ICMP_CMDLISTENER_H
