#ifndef ICMP_WORKER_H
#define ICMP_WORKER_H


#include <memory>
#include <thread>

namespace core
{
  using namespace std;

  class Worker {
  public:
    virtual ~Worker() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
  protected:
    void assign(function<void()> fn);
    void run();
    void terminate();
    function<void()> fn_;
    bool is_abort_ { false };
    unique_ptr<thread> thread_;
  };
} // core


#endif // ICMP_WORKER_H
