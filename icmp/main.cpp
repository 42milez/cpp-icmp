#include <iostream>
#include <memory>
#include <stdexcept>

#include <signal.h>

#include <boost/program_options.hpp>

#include "icmp/buildinfo.h"
#include "libconfig/Config.h"

using namespace std;

namespace po = boost::program_options;

namespace
{
  void version() {
    const auto* buildinfo = icmp_get_buildinfo();
    cout << "icmp " << buildinfo->project_version << "\n";
    cout << "Build: " << buildinfo->system_name << "/" << buildinfo->build_type << endl;
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
  po::options_description default_mode("DEFAULT MODE", 160);
  auto add_default_option = default_mode.add_options();
  add_default_option("-c,--config", po::value<string>()->value_name("<file>"),
                     "Configure specialised network using given JSON information\n");
  add_default_option("-h,--help", "Show this help message and exit\n");

  po::options_description allowed_options("Allowed options");
  allowed_options.add(default_mode);

  po::variables_map vm;
  vector<string> unrecognisedOptions;
  try {
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(allowed_options).allow_unregistered().run();
    unrecognisedOptions = collect_unrecognized(parsed.options, po::include_positional);
    po::store(parsed, vm);
    po::notify(vm);
  }
  catch (po::error const& e) {
    cerr << e.what();
    return -1;
  }

  for (const auto& option : unrecognisedOptions) {
    cerr << "Invalid argument: " << option << "\n";
    return -1;
  }

  if (vm.count("help")) {
    cout << "NAME:\n"
         << "   aleth " << icmp_get_buildinfo()->project_version << '\n'
         << "USAGE:\n"
         << "   aleth [options]\n\n"
         << "WALLET USAGE:\n";
    cout << default_mode;
    return 0;
  }

  signal(SIGABRT, &ExitHandler::exitHandler);
  signal(SIGTERM, &ExitHandler::exitHandler);
  signal(SIGINT, &ExitHandler::exitHandler);

  ExitHandler exitHandler;
  unique_ptr<config::Config> config;

  try {
    config = make_unique<config::Config>(argv[1]);
  } catch (invalid_argument const& e) {
    cerr << "Invalid parameter: " << e.what() << endl;
    return -1;
  }

  while (!exitHandler.shouldExit()) {
    // do something
    // ...
  }

  return 0;
}
