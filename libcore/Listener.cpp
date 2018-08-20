#include <iostream>

#include "Listener.h"

using namespace core;

Listener::Listener(std::shared_ptr<cfg::Config> config) {
  config_ = config;
  eth_ = std::make_unique<nw::EthernetIO>(config_);
  listener_ = std::make_unique<nw::RawSocketIO>(config_->device());
  logger_ = spdlog::stdout_color_mt("Listener");
  assign([this]{
    listener_->wait([this](const int fd){
      if (auto len = read(fd, buf_, sizeof(buf_)) <= 0) {
        this->logger_->error("read");
      } else {
        this->eth_->recv(buf_, len);
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
