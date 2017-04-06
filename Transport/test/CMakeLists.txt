project(transporttest)
cmake_minimum_required(VERSION 2.8)

file(GLOB PKG_SOURCE ServerTest.cpp)
add_executable(server_test ${PKG_SOURCE})
target_link_libraries(server_test transport utilities)

file(GLOB PKG_SOURCE ClientTest.cpp)
add_executable(client_test ${PKG_SOURCE})
target_link_libraries(client_test transport utilities)