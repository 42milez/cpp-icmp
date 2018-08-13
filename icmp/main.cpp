#include <iostream>
#include <icmp/buildinfo.h>

#include "libconfig/Config.h"

namespace {
    void version() {
      const auto* buildinfo = icmp_get_buildinfo();
      std::cout << "icmp " << buildinfo->project_version << "\n";
      std::cout << "Build: " << buildinfo->system_name << "/" << buildinfo->build_type << "\n";
    }
} // namespace

int main(int argc, char** argv) {
  version();
  return 0;
}
