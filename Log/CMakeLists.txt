project(Log)
cmake_minimum_required(VERSION 2.8)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.54 COMPONENTS thread filesystem regex system date_time program_options log log_setup REQUIRED)

include_directories(${Boost_INCLUDE_DIRS}) 

file(GLOB PKG_SOURCE *.cpp)
add_library(log STATIC ${PKG_SOURCE})
target_link_libraries(log  ${Boost_LIBRARIES})

add_subdirectory(test)