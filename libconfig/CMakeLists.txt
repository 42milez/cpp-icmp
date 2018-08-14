file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_library(config STATIC ${sources} ${headers})

target_link_libraries(config PRIVATE network Boost::boost)
