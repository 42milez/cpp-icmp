cmake_minimum_required(VERSION 3.10.3)

include(cmake/cable/bootstrap.cmake)
include(CableBuildInfo)
include(CableBuildType)
include(CableToolchains)

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.11.tar.gz"
    SHA1 "b732407d782459f89950ab52bc0ff9612f3e4be2"
    LOCAL
)

project(icmp)
set(PROJECT_VERSION 0.01)

cable_configure_toolchain(DEFAULT cxx14)
cable_set_build_type(DEFAULT RelWithDebInfo CONFIGURATION_TYPES Debug Release RelWithDebInfo)
cable_add_buildinfo_library(PROJECT_NAME icmp)

add_subdirectory(icmp)
