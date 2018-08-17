#include "Listener.h"

using namespace core;

Listener::Listener(const std::string device) {
  listener_ = std::make_unique<nw::RawSocketIO>(device);
  assign([this]{
    listener_->wait([this](){
      // ...
    });
  });
}

Listener::~Listener() {
  stop();
}

void Listener::start() {
  run();
}

void Listener::stop() {
  terminate();
}
