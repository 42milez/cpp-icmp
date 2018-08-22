#ifndef ICMP_WORKER_H
#define ICMP_WORKER_H


#include <functional>
#include <memory>
#include <thread>

namespace core
{
  class Worker {
  public:
    virtual ~Worker() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
  protected:
    void assign(std::function<void()> fn);
    void run();
    void terminate();
    bool is_abort_ { false };
    std::function<void()> fn_;
    std::unique_ptr<std::thread> thread_;
  };
} // core


#endif // ICMP_WORKER_H
