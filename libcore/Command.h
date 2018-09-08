#ifndef ICMP_COMMAND_H
#define ICMP_COMMAND_H


#include <spdlog/spdlog.h>

#include "libnetwork/RawSocket.h"
#include "Worker.h"

namespace core
{
  class Command : public Worker {
  public:
    Command();
    ~Command();
    void start() override;
    void stop() override;
  private:
    void wait();
    std::shared_ptr<spdlog::logger> logger_;
  };
}


#endif // ICMP_COMMAND_H
