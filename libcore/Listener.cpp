#include "Listener.h"

using namespace core;

Listener::Listener(const std::string device) {
  listener_ = std::make_unique<nw::RawSocketIO>(device);
  assign([this]{
    listener_->wait([this](const int fd){
      if (auto len = read(fd, buf_, sizeof(buf_)) <= 0) {
        this->logger_->error("read");
      } else {
        eth_->recv(buf_, len);
      }
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
