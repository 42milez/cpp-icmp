#ifndef ICMP_CONFIG_H
#define ICMP_CONFIG_H


#include <memory>
#include <string>

#include <netinet/if_ether.h>

#include "libnetwork/MAC.h"
#include "libnetwork/IP.h"

namespace config
{
  namespace nw = network;

  using Param = struct {
    std::unique_ptr<nw::IP> gateway;
    std::unique_ptr<nw::IP> vip;
    std::unique_ptr<nw::IP> vmask;
    std::unique_ptr<nw::MAC> vmac;
    std::string device;
    int mtu;
    int ttl;
  };

  const int DEFAULT_MTU = ETHERMTU;
  const int DEFAULT_TTL = 64;

  class Config {
  public:
    Config(const std::string& file_path);
    int is_target_ip_addr(u_int32_t addr);
    int is_same_subnet(const struct in_addr *addr);
    std::string device();
    const std::unique_ptr<nw::IP> & vip();
    const std::unique_ptr<nw::IP> & vmask();
    const std::unique_ptr<nw::MAC>& vmac();
  private:
    void read_param(const std::string& fname);
    std::unique_ptr<Param> param_;
  };
} // config


#endif // ICMP_CONFIG_H
