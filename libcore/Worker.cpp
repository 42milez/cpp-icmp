#include "Worker.h"

using namespace core;

void Worker::assign(function<void()> fn) {
  fn_ = move(fn);
}

void Worker::run() {
  if (fn_) {
    thread_ = make_unique<thread>([&fn = fn_, &is_abort = is_abort_] {
      for (;;) {
        if (is_abort) break;
        fn();
        this_thread::sleep_for(chrono::milliseconds(1000));
      }
    });
  }
}

void Worker::terminate() {
  if (thread_ && thread_->joinable()) {
    is_abort_ = true;
    thread_->join();
  }
}
