#include <unistd.h>

#include "NetworkIO.h"

using namespace network;

NetworkIO::NetworkIO(int port) {
  port_ = port;
  create_socket();
  create_multiplexer();
}

NetworkIO::~NetworkIO() {
  close(fd_);
}

void NetworkIO::create_socket() {
  // ...
}

void NetworkIO::create_multiplexer() {
  // ...
}

void NetworkIO::wait_for_accept(const function<void(const int)> &fn) {
  // ...
}

void NetworkIO::wait_for_read(const function<void()> &fn) {
  // ...
}
