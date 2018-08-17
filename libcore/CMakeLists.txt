file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_library(core STATIC ${sources} ${headers})

target_link_libraries(core PRIVATE util network spdlog::spdlog)
