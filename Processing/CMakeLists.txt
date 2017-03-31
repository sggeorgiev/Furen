project(Processing)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
include_directories(${Boost_INCLUDE_DIRS}) 

file(GLOB PKG_SOURCE *.cpp)
add_library(processing STATIC ${PKG_SOURCE})
target_link_libraries(processing  ${Boost_LIBRARIES})