#ifndef ICMP_WATCHER_H
#define ICMP_WATCHER_H


#include "Worker.h"

namespace core
{
  class Watcher : Worker {
  public:
    explicit Watcher();
    ~Watcher();
    void start() override;
    void stop() override;
  };
}


#endif // ICMP_WATCHER_H
