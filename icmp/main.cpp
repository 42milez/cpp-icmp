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
  po::options_description opt_desc("OPTIONS", 160);
  opt_desc.add_options()
    ("config,c", po::value<string>()->value_name("<file>"),
     "Configure specialised network using given JSON information\n")
    ("help,h", "Show this help message and exit\n");

  po::options_description allowed_options("Allowed options");
  allowed_options.add(opt_desc);

  po::variables_map vm;
  vector<string> unrecognised_options;
  try {
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(allowed_options).allow_unregistered().run();
    unrecognised_options = collect_unrecognized(parsed.options, po::include_positional);
    po::store(parsed, vm);
    po::notify(vm);
  } catch (po::error const &e) {
    cerr << e.what();
    return -1;
  }

  for (const auto &option : unrecognised_options) {
    cerr << "Invalid argument: " << option << "\n";
    return -1;
  }

  if (vm.count("h") || vm.count("help")) {
    cout << "NAME:" << endl
         << "   icmp " << icmp_get_buildinfo()->project_version << endl << endl
         << "USAGE:" << endl
         << "   icmp [options]" << endl << endl;
    cout << opt_desc;
    return 0;
  }

  unique_ptr<config::Config> config;

  if (vm.count("config")) {
    try {
      config = make_unique<config::Config>(vm["config"].as<string>());
    } catch (invalid_argument const &e) {
      cerr << "Invalid parameter: " << e.what() << endl;
      return -1;
    }
  }

  signal(SIGABRT, &ExitHandler::exitHandler);
  signal(SIGTERM, &ExitHandler::exitHandler);
  signal(SIGINT, &ExitHandler::exitHandler);

  ExitHandler exitHandler;

  while (!exitHandler.shouldExit()) {
    // do something
    // ...
  }

  return 0;
}
