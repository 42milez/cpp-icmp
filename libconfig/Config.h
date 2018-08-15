#ifndef ICMP_CONFIG_H
#define ICMP_CONFIG_H


#include <memory>
#include <string>

#include <netinet/if_ether.h>
#include <netinet/in.h>

#include "libnetwork/MAC.h"

namespace config
{
  using namespace std;

  namespace nw = network;

  using InAddr = struct in_addr;

  using Param = struct {
    InAddr gateway;
    InAddr ip;
    InAddr vip;
    InAddr vmask;
    string device;
    unique_ptr<nw::MAC> mac;
    unique_ptr<nw::MAC> vmac;
    int mtu;
    int ttl;
  };

  const int DEFAULT_MTU = ETHERMTU;
  const int DEFAULT_TTL = 64;

  class Config {
  public:
    Config(const string& file_path);
    int is_target_ip_addr(InAddr &in_addr);
    int is_same_subnet(InAddr &addr);
  private:
    void read_param(const string& fname);
    unique_ptr<Param> param_;
  };
} // config


#endif //ICMP_CONFIG_H
