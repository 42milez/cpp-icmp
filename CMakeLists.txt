cmake_minimum_required(VERSION 3.12.1)

include(cmake/cable/bootstrap.cmake)
include(CableBuildInfo)
include(CableBuildType)
include(CableToolchains)

cable_configure_toolchain(DEFAULT cxx17)

set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "Hunter configuration types")
set(HUNTER_JOBS_NUMBER 4 CACHE STRING "Hunter jobs number")

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.11.tar.gz"
    SHA1 "b732407d782459f89950ab52bc0ff9612f3e4be2"
    LOCAL
)

project(icmp)
set(PROJECT_VERSION 0.0.1)

cable_set_build_type(DEFAULT RelWithDebInfo CONFIGURATION_TYPES Debug Release RelWithDebInfo)
cable_add_buildinfo_library(PROJECT_NAME icmp)

hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(libconfig)
add_subdirectory(libnetwork)

add_subdirectory(icmp)
