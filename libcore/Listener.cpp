#include "Listener.h"

using namespace core;

Listener::Listener() {
  listener_ = make_unique<nw::NetworkIO>();
  assign([this]{
    listener_->wait_for_read([this](){
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
