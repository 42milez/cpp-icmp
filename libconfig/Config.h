#ifndef ICMP_CONFIG_H
#define ICMP_CONFIG_H


#include <memory>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <string>

#include "libnetwork/MAC.h"

namespace config
{
  using std::string;
  using std::unique_ptr;

  using network::MAC;

  using InAddr = struct in_addr;

  using Param = struct {
    InAddr gateway;
    InAddr ip;
    InAddr vip;
    InAddr vmask;
    string device;
    unique_ptr<MAC> mac;
    unique_ptr<MAC> vmac;
    int mtu;
    int ttl;
  };

  const int DEFAULT_MTU = ETHERMTU;
  const int DEFAULT_TTL = 64;

  class Config {
  public:
    Config();
    int is_target_ip_addr(InAddr &in_addr);
    int is_same_subnet(InAddr &addr);
  private:
    void read_param(const string& fname);
    void set_default_param();
    unique_ptr<Param> param_;
  };
}


#endif //ICMP_CONFIG_H
