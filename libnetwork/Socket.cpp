#include "Socket.h"

const int Socket::NEVENTS = 16;

Socket::~Socket() {
  close(fd_);
}
