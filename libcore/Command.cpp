#include "Command.h"

using namespace core;

Command::Command() {
  logger_ = spdlog::stdout_color_mt("Command");
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

void Command::wait() {
#if defined(__linux__)
  // ...
#else
  // Unix
  // ...
#endif
}
