file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_library(param STATIC ${sources} ${headers})
