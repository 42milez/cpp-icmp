#include <arpa/inet.h>

#include <boost/property_tree/ini_parser.hpp>

#include "Config.h"

using std::make_unique;

using boost::property_tree::ptree;

using namespace config;

Config::Config() {
  param_ = std::make_unique<Param>();
  read_param("network.ini");
  set_default_param();
}

int Config::is_target_ip_addr(InAddr &in_addr) {
  return 0;
}

int Config::is_same_subnet(InAddr &addr) {
  return 0;
}

void Config::read_param(const string& filename) {
  ptree pt;
  read_ini(filename, pt);

  if (auto val = pt.get_optional<string>("Network.gateway")) {
    param_->gateway.s_addr = inet_addr(val.get().c_str());
  } else {
    param_->gateway.s_addr = 0;
  }

  if (auto val = pt.get_optional<string>("Network.vip")) {
    param_->vip.s_addr = inet_addr(val.get().c_str());
  } else {
    param_->vip.s_addr = 0;
  }

  if (auto val = pt.get_optional<string>("Network.vmask")) {
    param_->vmask.s_addr = inet_addr(val.get().c_str());
  } else {
    param_->vmask.s_addr = 0;
  }

  if (auto val = pt.get_optional<string>("Network.device")) {
    param_->device = val.get();
  } else {
    param_->device = "";
  }

  if (auto val = pt.get_optional<string>("Network.vmac")) {
    param_->vmac = make_unique<MAC>(val.get());
  }

  if (auto val = pt.get_optional<int>("Network.mtu")) {
    param_->mtu = val.get();
  } else {
    param_->mtu = -1;
  }

  if (auto val = pt.get_optional<int>("Network.ttl")) {
    param_->ttl = val.get();
  } else {
    param_->ttl = -1;
  }
}

void Config::set_default_param() {
  if (param_->mtu == -1) {
    param_->mtu = DEFAULT_MTU;
  }

  if (param_->ttl == -1) {
    param_->ttl = DEFAULT_TTL;
  }
}
