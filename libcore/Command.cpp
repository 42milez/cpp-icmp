#include <cstdio>
#include <iostream>

#if defined(__linux__)
#include <sys/epoll.h>
#else
  // Unix
  // ...
#endif

#include "libutil/InternalErrorException.h"
#include "Command.h"

using namespace core;

using bytes = std::vector<std::byte>;

Command::Command() {
  logger_ = spdlog::stdout_color_mt("Command");
  setup_multiplexer();
  assign([this]{ this->wait(); });
}

Command::~Command() {
  stop();
}

void Command::start() {
  run();
}

void Command::stop() {
  terminate();
}

void Command::setup_multiplexer() {
#if defined(__linux__)
  mux_ = epoll_create(N_EVENTS);
  if (mux_ < 0) {
    logger_->error("epoll_create");
    throw util::InternalErrorException{ "Cannot create event poll." };
  }

  struct epoll_event ev { 0 };
  ev.events = EPOLLIN;
  ev.data.fd = fileno(stdin);
  if (epoll_ctl(mux_, EPOLL_CTL_ADD, fileno(stdin), &ev)) {
    logger_->error("epoll_ctl");
    throw util::InternalErrorException{ "Cannot create event poll." };
  }
#else
  // Unix
  // ...
#endif
}

void Command::wait() {
#if defined(__linux__)
  int nfds = epoll_wait(mux_, events_.data(), N_EVENTS, -1);
  if (nfds <= 0) {
    logger_->error("epoll_wait");
    return;
  }

  for (auto i = 0; i < nfds; i++) {
    if (events_[i].data.fd == fileno(stdin)) {
      bytes buf = bytes(SIZE_BUFFER);
      fgets(reinterpret_cast<char *>(buf.data()), sizeof(buf.data()), stdin);
      exec(buf);
    }
  }
#else
  // Unix
  // ...
#endif
}

void Command::exec(const bytes &buf) {
  // ...
}
