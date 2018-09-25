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
#include "libcore/CmdListener.h"
#include "libcore/EthListener.h"

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
    static void exit() { s_shouldExit = true; }
    bool shouldExit() const { return s_shouldExit; }
  private:
    static bool s_shouldExit;
  };

  bool ExitHandler::s_shouldExit = false;

  template <typename Func>
  void register_handler(int signum, Func handler) {
    struct sigaction act { nullptr };
    sigset_t set;

    act.sa_handler = handler;
    act.sa_flags = 0;

    sigemptyset(&set);
    sigaddset(&set, signum);

    act.sa_mask = set;
    sigaction(signum, nullptr, &act);
  }
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

  // Signal Handling
  // - http://doi-t.hatenablog.com/entry/2014/02/15/020909
  // - http://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
  // - http://kzth.sakura.ne.jp/memo/?p=77

  // Handle Exit Signal
  auto sig_hander = [](int signum) { ExitHandler::exit(); };
  register_handler(SIGABRT, sig_hander);
  register_handler(SIGINT, sig_hander);
  register_handler(SIGTERM, sig_hander);

  // Handle Pipe Signal
  register_handler(SIGPIPE, SIG_IGN);

  std::unique_ptr<core::CmdListener> command;
  std::unique_ptr<core::EthListener> listener;

  try {
    command = std::make_unique<core::CmdListener>(config->device());
    listener = std::make_unique<core::EthListener>(config);
  } catch (util::InternalErrorException const& e) {
    logger->critical(e.what());
    return -1;
  }

  command->start();
  listener->start();

  ExitHandler eh;

  while (!eh.shouldExit()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  command->stop();
  listener->stop();

  return 0;
}
