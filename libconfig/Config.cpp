#include "Config.h"

using namespace config;

int Config::set_default_param() {
  return 0;
}

int Config::read_param(std::string &fname) {
  return 0;
}

int Config::is_target_ip_addr(param::InAddr &in_addr) {
  return 0;
}

int Config::is_same_subnet(param::InAddr &addr) {
  return 0;
}
