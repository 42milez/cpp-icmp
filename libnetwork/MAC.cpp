#include <regex>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "MAC.h"

using namespace network;

const std::string MAC_ADDRESS_DELIMITER = ":";
const std::string MAC_ADDRESS_PATTERN = "[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}";

MAC::MAC(const std::string& mac)
{
  std::regex pattern(MAC_ADDRESS_PATTERN);
  std::smatch match;

  if (!std::regex_match(mac, match, pattern)) {
    throw std::invalid_argument("Invalid MAC address.");
  }

  mac_ = mac;
};

std::vector<long> MAC::as_hex()
{
  std::vector<std::string> splitted;
  boost::split(splitted, mac_, boost::is_any_of(MAC_ADDRESS_DELIMITER));

  std::vector<long> octets;
  octets.reserve(6);

  for (auto const& oct : splitted) {
    octets.push_back(strtol(oct.c_str(), nullptr, 16));
  }

  return octets;
}

const std::string& MAC::as_str()
{
  return mac_;
}
