project(server)

set(CMAKE_CXX_FLAGS "-g -Wall")
add_executable(server ClientInfo.cpp main.cpp)


add_subdirectory(test)

include(CTest)