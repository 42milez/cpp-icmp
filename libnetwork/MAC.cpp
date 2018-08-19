#include <regex>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "MAC.h"

using namespace network;

namespace bst = boost;

const std::string MAC::MAC_ADDRESS_DELIMITER = ":";
const std::string MAC::MAC_ADDRESS_PATTERN = "[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}";

MAC::MAC(const std::string mac) {
  std::regex pattern(MAC_ADDRESS_PATTERN);
  std::smatch match;

  if (std::regex_match(mac, match, pattern)) {
    bst::split(mac_, mac, bst::is_any_of(MAC_ADDRESS_DELIMITER));
  } else {
    throw std::invalid_argument("Invalid MAC address.");
  }
};

std::unique_ptr<std::string> MAC::mac() {
  std::ostringstream os;
  std::copy(mac_.begin(), mac_.end(), std::ostream_iterator<std::string>(os, ":"));
  return std::move(std::make_unique<std::string>(os.str()));
}
