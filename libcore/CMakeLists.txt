file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_library(core STATIC ${sources} ${headers})
