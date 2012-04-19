project(client)

set(CMAKE_CXX_FLAGS "-g -Wall")
add_executable(client main.cpp ../server/util.cpp)

