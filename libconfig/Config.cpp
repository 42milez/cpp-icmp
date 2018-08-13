#include "Config.h"

using namespace config;

Config::Config() {
  param_ = std::make_unique<config::Param>();
}

int Config::is_target_ip_addr(InAddr &in_addr) {
  return 0;
}

int Config::is_same_subnet(InAddr &addr) {
  return 0;
}

int Config::set_default_param() {
  return 0;
}

int Config::read_param(string &fname) {
  return 0;
}
