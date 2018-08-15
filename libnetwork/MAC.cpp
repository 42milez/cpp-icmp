#include <regex>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "MAC.h"

using namespace std;

using namespace network;

namespace bst = boost;

const string MAC::MAC_ADDRESS_DELIMITER = ":";
const string MAC::MAC_ADDRESS_PATTERN = "[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}";

MAC::MAC(const string mac) {
  regex pattern(MAC_ADDRESS_PATTERN);
  smatch match;

  if (regex_match(mac, match, pattern)) {
    bst::split(mac_, mac, bst::is_any_of(MAC_ADDRESS_DELIMITER));
  } else {
    throw invalid_argument("Invalid MAC address.");
  }
};

MAC::MAC(string& oct1, string& oct2, string& oct3, string& oct4, string& oct5, string& oct6) {
  mac_ = { move(oct1), move(oct2), move(oct3), move(oct4), move(oct5), move(oct6) };
}
