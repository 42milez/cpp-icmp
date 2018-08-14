#include <iostream>
#include <memory>
#include <stdexcept>

#include <signal.h>

#include <icmp/buildinfo.h>
#include "libconfig/Config.h"

using std::cout;
using std::endl;
using std::invalid_argument;
using std::make_unique;
using std::unique_ptr;

namespace
{
  void version() {
    const auto* buildinfo = icmp_get_buildinfo();
    std::cout << "icmp " << buildinfo->project_version << "\n";
    std::cout << "Build: " << buildinfo->system_name << "/" << buildinfo->build_type << "\n";
  }

  class ExitHandler {
  public:
    static void exitHandler(int) { s_shouldExit = true; }
    bool shouldExit() const { return s_shouldExit; }
  private:
    static bool s_shouldExit;
  };

  bool ExitHandler::s_shouldExit = false;
} // namespace

int main(int argc, char** argv) {
  signal(SIGABRT, &ExitHandler::exitHandler);
  signal(SIGTERM, &ExitHandler::exitHandler);
  signal(SIGINT, &ExitHandler::exitHandler);

  ExitHandler exitHandler;
  unique_ptr<config::Config> config;

  try {
    config = make_unique<config::Config>();
  } catch (invalid_argument& e) {
    cout << "Error: " << e.what() << endl;
    return -1;
  }

  while (!exitHandler.shouldExit()) {
    // do something
    // ...
  }

  return 0;
}
