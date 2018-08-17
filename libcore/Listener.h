#ifndef ICMP_LISTENER_H
#define ICMP_LISTENER_H


#include <memory>
#include <string>

#include "libnetwork/RawSocketIO.h"

#include "Worker.h"

namespace core
{
  namespace nw = network;

  class Listener : public Worker {
  public:
    explicit Listener(const std::string device);
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    std::unique_ptr<nw::RawSocketIO> listener_;
  };
} // core


#endif // ICMP_LISTENER_H
