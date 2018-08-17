#ifndef ICMP_MAC_H
#define ICMP_MAC_H


#include <string>
#include <vector>

namespace network
{
  using std::string;
  using std::vector;

  class MAC {
  public:
    explicit MAC(string mac);
    MAC(string& oct1, string& oct2, string& oct3, string& oct4, string& oct5, string& oct6);
  private:
    static const string MAC_ADDRESS_DELIMITER;
    static const string MAC_ADDRESS_PATTERN;

    vector<string> mac_;
  };
} // network


#endif // ICMP_MAC_H
