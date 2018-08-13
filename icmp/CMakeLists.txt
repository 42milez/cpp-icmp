add_executable(icmp main.cpp)

target_link_libraries(icmp PRIVATE icmp-buildinfo config)
