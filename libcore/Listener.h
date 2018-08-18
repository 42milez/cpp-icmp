#ifndef ICMP_LISTENER_H
#define ICMP_LISTENER_H


#include <memory>
#include <string>

#include <spdlog/spdlog.h>

#include "libnetwork/RawSocketIO.h"
#include "Worker.h"

namespace core
{
  namespace nw = network;

  class Listener : public Worker {
  public:
    explicit Listener(std::string device);
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    std::unique_ptr<nw::RawSocketIO> listener_;
    u_int8_t buf_[2048];

    std::shared_ptr<spdlog::logger> logger_;
  };
} // core


#endif // ICMP_LISTENER_H
