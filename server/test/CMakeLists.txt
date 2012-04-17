cmake_minimum_required(VERSION 2.8)
ENABLE_TESTING()
project(test)
 
add_executable(testClientInfo testClientInfo.cpp ../ClientInfo.cpp)

add_test(StocheazaClientii testClientInfo "goagu" "12.77.0.1" "5544")
set_tests_properties(StocheazaClientii
   PROPERTIES PASS_REGULAR_EXPRESSION "gogu: port: 4424 time elapsed: 2\ngoagu: port: 5544 time elapsed: 2")

