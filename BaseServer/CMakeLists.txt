project(BaseServer)
cmake_minimum_required(VERSION 2.8)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB PKG_SOURCE *.cpp)
<<<<<<< Updated upstream:BaseServer/CMakeLists.txt
add_library(BaseServer STATIC ${PKG_SOURCE})
target_link_libraries(BaseServer  ${Boost_LIBRARIES} utilities log)
=======
add_library(transport STATIC ${PKG_SOURCE})
target_link_libraries(transport  ${Boost_LIBRARIES} utilities log processing)
add_subdirectory(test)
>>>>>>> Stashed changes:Transport/CMakeLists.txt
