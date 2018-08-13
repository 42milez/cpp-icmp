#ifndef ICMP_CONFIG_H
#define ICMP_CONFIG_H

#include <memory>
#include <netinet/if_ether.h>
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
    int ttl;
    int mtu;
  };

  const int DEFAULT_MTU = ETHERMTU;
  const int DEFAULT_TTL = 64;

  class Config {
  public:
    Config();
    int is_target_ip_addr(InAddr &in_addr);
    int is_same_subnet(InAddr &addr);
  private:
    int set_default_param();
    int read_param(string &fname);
    unique_ptr<Param> param_;
  };
}


#endif //ICMP_CONFIG_H
