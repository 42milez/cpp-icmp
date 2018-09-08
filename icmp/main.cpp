#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

#include <signal.h>

#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

#include "libutil/InternalErrorException.h"
#include "icmp/buildinfo.h"
#include "libconfig/Config.h"
#include "libcore/Command.h"
#include "libcore/Listener.h"

namespace cfg = config;
namespace po = boost::program_options;

namespace
{
  void version() {
    const auto* buildinfo = icmp_get_buildinfo();
    std::cout << "icmp " << buildinfo->project_version << "\n";
    std::cout << "Build: " << buildinfo->system_name << "/" << buildinfo->build_type << std::endl;
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
  auto logger = spdlog::stdout_color_mt("ICMP");

  po::options_description opt_desc("OPTIONS", 160);
  opt_desc.add_options()
    ("config,c", po::value<std::string>()->value_name("<file>"),
     "Configure specialised network using given JSON information\n")
    ("help,h", "Show this help message and exit\n");

  po::options_description allowed_options("Allowed options");
  allowed_options.add(opt_desc);

  po::variables_map vm;
  std::vector<std::string> unrecognised_options;
  try {
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(allowed_options).allow_unregistered().run();
    unrecognised_options = collect_unrecognized(parsed.options, po::include_positional);
    po::store(parsed, vm);
    po::notify(vm);
  } catch (po::error const &e) {
    std::cerr << e.what();
    return -1;
  }

  if (argc <= 1) {
    std::cout << "Requires at least 1 argument." << std::endl << std::endl;
    std::cout << "USAGE:" << std::endl
              << "   icmp [options]" << std::endl << std::endl;
    std::cout << opt_desc;
    return -1;
  }

  for (const auto &option : unrecognised_options) {
    std::cerr << "Invalid argument: " << option << "\n";
    return -1;
  }

  std::shared_ptr<cfg::Config> config;

  if (vm.count("h") || vm.count("help")) {
    std::cout << "NAME:" << std::endl
         << "   icmp " << icmp_get_buildinfo()->project_version << std::endl << std::endl
         << "USAGE:" << std::endl
         << "   icmp [options]" << std::endl << std::endl;
    std::cout << opt_desc;
    return 0;
  } else if (vm.count("config")) {
    try {
      config = std::make_shared<config::Config>(vm["config"].as<std::string>());
    } catch (std::invalid_argument const &e) {
      std::cerr << "Invalid parameter: " << e.what() << std::endl;
      return -1;
    }
  }

  // todo: Use sigaction(2)
  signal(SIGABRT, &ExitHandler::exitHandler);
  signal(SIGTERM, &ExitHandler::exitHandler);
  signal(SIGINT, &ExitHandler::exitHandler);
  signal(SIGPIPE, SIG_IGN);

  ExitHandler exitHandler;

  std::unique_ptr<core::Command> command = std::make_unique<core::Command>();

  std::unique_ptr<core::Listener> listener;

  try {
    listener = std::make_unique<core::Listener>(config);
  } catch (util::InternalErrorException const& e) {
    logger->critical(e.what());
    return -1;
  }

  command->start();
  listener->start();

  while (!exitHandler.shouldExit()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  listener->stop();

  return 0;
}
