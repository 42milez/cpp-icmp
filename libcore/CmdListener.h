#ifndef ICMP_CMDLISTENER_H
#define ICMP_CMDLISTENER_H


#include <cstddef>

#include <spdlog/spdlog.h>

#include "libnetwork/RawSocket.h"
#include "Worker.h"

namespace core
{
  using bytes = std::vector<std::byte>;

#ifdef __linux__
  using EpEvt = struct epoll_event;
#endif

  class CmdListener : public Worker {
  public:
    CmdListener();
    ~CmdListener();
    void start() override;
    void stop() override;
  private:
    static const size_t N_EVENTS = 16;
    static const size_t SIZE_BUFFER = 2048;

    void setup_multiplexer();
    void wait();
    void parse(const std::string &cmd);
    void exec_arp(const std::string &option);

    std::shared_ptr<spdlog::logger> logger_;
    int mux_;
#ifdef __linux__
    std::array<EpEvt, N_EVENTS> events_;
#else
#endif
  };
}


#endif // ICMP_CMDLISTENER_H
