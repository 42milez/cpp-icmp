#include "Listener.h"

using namespace core;

Listener::Listener() {
  listener_ = make_unique<nw::RawSocket>();
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
