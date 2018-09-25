#include <cstdio>
#include <iostream>
#include <csignal>

#ifdef __linux__
#include <sys/epoll.h>
#else
  // Unix
  // ...
#endif

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "libutil/InternalErrorException.h"
#include "CmdListener.h"

using namespace core;

using bytes = std::vector<std::byte>;

CmdListener::CmdListener(std::string_view device)
{
  logger_ = spdlog::stdout_color_mt("CmdListener");
  sock_ = std::make_unique<nw::TcpSocket>(device);
  setup_multiplexer();
  assign([this]{ this->wait(); });
}

CmdListener::~CmdListener()
{
  stop();
}

void CmdListener::start()
{
  run();
}

void CmdListener::stop()
{
  terminate();
}

void CmdListener::setup_multiplexer()
{
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

void CmdListener::wait()
{
#if defined(__linux__)
  int nfds = epoll_wait(mux_, events_.data(), N_EVENTS, -1);
  if (nfds <= 0) {
    logger_->error("epoll_wait");
    return;
  }

  for (auto i = 0; i < nfds; i++) {
    if (events_[i].data.fd == fileno(stdin)) {
      std::string cmd;
//      bytes buf = bytes(SIZE_BUFFER);
//      fgets(reinterpret_cast<char *>(buf.data()), sizeof(buf.data()), stdin);
      std::cin >> cmd;
      parse(cmd);
    }
  }
#else
  // Unix
  // ...
#endif
}

void CmdListener::parse(const std::string &cmd)
{
  std::vector<std::string> splitted;
  boost::algorithm::split(splitted, cmd, boost::is_any_of(" "));

  if (splitted[0] == "arp") {
    // ...
  } else if (splitted[0] == "ping") {
    // ...
  } else if (splitted[0] == "exit") {
    // ...
  } else {
    logger_->error("Invalid command.");
  }
}

void CmdListener::exec_arp(const std::string &option)
{
  if (option == "-a") {

  }

  if (option == "-d") {

  }
}
