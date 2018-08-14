add_executable(icmp main.cpp)

target_link_libraries(icmp PRIVATE config icmp-buildinfo Boost::program_options)
