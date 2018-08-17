#include "Listener.h"

using namespace core;

using std::make_unique;

Listener::Listener(const string device) {
  listener_ = make_unique<nw::RawSocketIO>(device);
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
