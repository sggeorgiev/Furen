project(daemontest)
cmake_minimum_required(VERSION 2.8)

file(GLOB PKG_SOURCE *.cpp)
add_executable(daemon_test ${PKG_SOURCE})
target_link_libraries(daemon_test daemon log)