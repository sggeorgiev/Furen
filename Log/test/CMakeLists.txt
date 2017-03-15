project(logtest)
cmake_minimum_required(VERSION 2.8)

file(GLOB PKG_SOURCE *.cpp)
add_executable(log_test ${PKG_SOURCE})
target_link_libraries(log_test log)