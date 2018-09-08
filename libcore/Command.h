#ifndef ICMP_COMMAND_H
#define ICMP_COMMAND_H


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

  class Command : public Worker {
  public:
    Command();
    ~Command();
    void start() override;
    void stop() override;
  private:
    static const size_t N_EVENTS = 16;
    static const size_t SIZE_BUFFER = 2048;

    void exec(const bytes &buf);
    void setup_multiplexer();
    void wait();

    std::shared_ptr<spdlog::logger> logger_;
    int mux_;
#ifdef __linux__
    std::array<EpEvt, N_EVENTS> events_;
#else
#endif
  };
}


#endif // ICMP_COMMAND_H
