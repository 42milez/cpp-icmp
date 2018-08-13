#ifndef ICMP_CONFIG_H
#define ICMP_CONFIG_H


#include <netinet/in.h>
#include <string>

namespace config
{
  using namespace std;

  using InAddr = struct in_addr;

  using Param = struct {
    string device;
    unsigned mymac[6];
    unsigned vmac[6];
    InAddr myip;
    InAddr vip;
    InAddr vmask;
    InAddr gateway;
    int IpTTL;
    int MTU;
  };

  class Config {
  public:
    int set_default_param();
    int read_param(string &fname);
    int is_target_ip_addr(InAddr &in_addr);
    int is_same_subnet(InAddr &addr);
  };
}


#endif //ICMP_CONFIG_H
