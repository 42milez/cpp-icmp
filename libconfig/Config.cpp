#include <stdexcept>

#include <arpa/inet.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Config.h"

using namespace config;

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;
namespace nw = network;

using std::invalid_argument;
using std::make_unique;

Config::Config(const string& file_path) {
  if (!fs::exists(file_path)) {
    throw invalid_argument("Invalid file path.");
  }
  param_ = make_unique<Param>();
  read_param(file_path);
}

int Config::is_target_ip_addr(InAddr &in_addr) {
  return 0;
}

int Config::is_same_subnet(InAddr &addr) {
  return 0;
}

void Config::read_param(const string& filename) {
  pt::ptree prop;
  read_json(filename, prop);

  if (auto val = prop.get_optional<string>("network.gateway")) {
    param_->gateway.s_addr = inet_addr(val.get().c_str());
  } else {
    throw invalid_argument("Invalid gateway address.");
  }

  if (auto val = prop.get_optional<string>("network.vip")) {
    param_->vip.s_addr = inet_addr(val.get().c_str());
  } else {
    throw invalid_argument("Invalid virtual IP address.");
  }

  if (auto val = prop.get_optional<string>("network.vmask")) {
    param_->vmask.s_addr = inet_addr(val.get().c_str());
  } else {
    throw invalid_argument("Invalid virtual netmask.");
  }

  if (auto val = prop.get_optional<string>("network.device")) {
    param_->device = val.get();
  } else {
    throw invalid_argument("Invalid device name.");
  }

  if (auto val = prop.get_optional<string>("network.vmac")) {
    param_->vmac = make_unique<nw::MAC>(val.get());
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

string Config::device() {
  return param_->device;
}
