#ifndef ICMP_CONFIG_H
#define ICMP_CONFIG_H


#include <memory>
#include <string>

#include <netinet/if_ether.h>
#include <netinet/in.h>

#include "libnetwork/MAC.h"

namespace config
{
  namespace nw = network;

  using InAddr = struct in_addr;

  using Param = struct {
    InAddr gateway;
    InAddr ip;
    InAddr vip;
    InAddr vmask;
    std::string device;
    std::unique_ptr<nw::MAC> mac;
    std::unique_ptr<nw::MAC> vmac;
    int mtu;
    int ttl;
  };

  const int DEFAULT_MTU = ETHERMTU;
  const int DEFAULT_TTL = 64;

  class Config {
  public:
    Config(const std::string& file_path);
    std::string device();
    const InAddr& vip();
    const std::unique_ptr<nw::MAC>& vmac();
    const InAddr& vmask();
  private:
    void read_param(const std::string& fname);
    std::unique_ptr<Param> param_;
  };
} // config


#endif // ICMP_CONFIG_H
