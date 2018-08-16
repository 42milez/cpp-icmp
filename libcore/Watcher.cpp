#include "Watcher.h"

using namespace core;

Watcher::Watcher() {
  assign([this]{
    // ...
  });
}

Watcher::~Watcher() {
  stop();
}

void Watcher::start() {
  run();
}

void Watcher::stop() {
  terminate();
}
