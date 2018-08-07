cmake_minimum_required(VERSION 3.10.3)

include(cmake/cable/bootstrap.cmake)
include(CableBuildInfo)
include(CableBuildType)
include(CableToolchains)

project(icmp)
set(PROJECT_VERSION 0.01)

cable_configure_toolchain(DEFAULT cxx14)
cable_set_build_type(DEFAULT RelWithDebInfo CONFIGURATION_TYPES Debug Release RelWithDebInfo)
cable_add_buildinfo_library(PROJECT_NAME icmp)

#message(STATUS "*** dump start cmake variables ***")
#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS "*** dump end ***")

add_subdirectory(icmp)
