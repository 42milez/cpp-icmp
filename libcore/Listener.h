#ifndef ICMP_LISTENER_H
#define ICMP_LISTENER_H


#include <memory>

#include "NetworkIO.h"
#include "Worker.h"

namespace core
{
  namespace nw = network;

  class Listener : public Worker {
  public:
    explicit Listener();
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    unique_ptr<nw::NetworkIO> listener_;
  };
} // core


#endif // ICMP_LISTENER_H
