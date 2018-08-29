#include <stdexcept>

#include <arpa/inet.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Config.h"

using namespace config;

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;
namespace nw = network;

Config::Config(const std::string& file_path) {
  if (!fs::exists(file_path)) {
    throw std::invalid_argument("Invalid file path.");
  }
  param_ = std::make_unique<Param>();
  read_param(file_path);
}

std::string Config::device() {
  return param_->device;
}

const std::unique_ptr<nw::IP> & Config::vip() {
  return param_->vip;
}

const std::unique_ptr<nw::IP> & Config::vmask() {
  return param_->vmask;
}

const std::unique_ptr<nw::MAC> & Config::vmac() {
  return param_->vmac;
}

int Config::is_target_ip_addr(const struct in_addr *addr) {
  if (param_->vip->as_numeric() == addr->s_addr) {
    return 1;
  }
  return 0;
}

int Config::is_same_subnet(const struct in_addr *addr) {
  if ((addr->s_addr & param_->vmask->as_numeric()) == (param_->vip->as_numeric() & param_->vmask->as_numeric())) {
    return 1;
  }
  else {
    return 0;
  }
}

void Config::read_param(const std::string& filename) {
  pt::ptree prop;
  read_json(filename, prop);

  if (auto val = prop.get_optional<std::string>("network.gateway")) {
    param_->gateway = std::make_unique<nw::IP>(val.get());
  } else {
    throw std::invalid_argument("Invalid gateway address.");
  }

  if (auto val = prop.get_optional<std::string>("network.vip")) {
    param_->vip = std::make_unique<nw::IP>(val.get());
  } else {
    throw std::invalid_argument("Invalid virtual ICMP address.");
  }

  if (auto val = prop.get_optional<std::string>("network.vmask")) {
    param_->vmask = std::make_unique<nw::IP>(val.get());
  } else {
    throw std::invalid_argument("Invalid virtual netmask.");
  }

  if (auto val = prop.get_optional<std::string>("network.vmac")) {
    param_->vmac = std::make_unique<nw::MAC>(val.get());
  }

  if (auto val = prop.get_optional<std::string>("network.device")) {
    param_->device = val.get();
  } else {
    throw std::invalid_argument("Invalid device name.");
  }

  if (auto val = prop.get_optional<int>("network.mtu")) {
    param_->mtu = val.get();
  } else {
    param_->mtu = DEFAULT_MTU;
  }

  if (auto val = prop.get_optional<int>("network.ttl")) {
    param_->ttl = val.get();
  } else {
    param_->ttl = DEFAULT_TTL;
  }
}
