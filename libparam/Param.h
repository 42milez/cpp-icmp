#ifndef ICMP_PARAM_H
#define ICMP_PARAM_H


#include <netinet/in.h>
#include <string>

namespace param {
  using InAddr = struct in_addr;
  using PARAM = struct {
    std::string device;
    unsigned mymac[6];
    InAddr myip;
    unsigned vmac[6];
    InAddr vip;
    InAddr vmask;
    int IpTTL;
    int MTU;
    InAddr gateway;
  };

  class Param {
  public:
    int set_default_param();
    int read_param(std::string& fname);
    int is_target_ip_addr(InAddr& in_addr);
    int is_same_subnet(InAddr& addr);
  };
}


#endif //ICMP_PARAM_H
