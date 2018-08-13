file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_library(config STATIC ${sources} ${headers})
