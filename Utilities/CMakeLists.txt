project(Utilities)
cmake_minimum_required(VERSION 2.8)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS}) 

file(GLOB PKG_SOURCE *.cpp)
add_library(utilities STATIC ${PKG_SOURCE})
target_link_libraries(utilities  ${Boost_LIBRARIES})