#include <regex>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "MAC.h"

using namespace network;

namespace bst = boost;

const std::string MAC_ADDRESS_DELIMITER = ":";
const std::string MAC_ADDRESS_PATTERN = "[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}";

MAC::MAC(const std::string& mac) {
  // store as string separated by each octet
  std::regex pattern(MAC_ADDRESS_PATTERN);
  std::smatch match;
  if (std::regex_match(mac, match, pattern)) {
    bst::split(mac_, mac, bst::is_any_of(MAC_ADDRESS_DELIMITER));
  } else {
    throw std::invalid_argument("Invalid MAC address.");
  }

  // store as hexadecimal separated by each octet
  for (auto const& oct : mac_) {
    mac_hex_.push_back(strtol(oct.c_str(), nullptr, 16));
  }

  // store as string joined by colon
  mac_str_ = mac;
};

const std::vector<u_int8_t>& MAC::as_hex() {
  return mac_hex_;
}

const std::string& MAC::as_str() {
  return mac_str_;
}
