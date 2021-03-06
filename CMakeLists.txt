project(Furen)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-std=gnu99 ")
set(CMAKE_C_FLAGS_DEBUG "-std=gnu99 -O0 -g -gdwarf-3 -Wall -Werror=implicit -Werror=implicit-int -Werror=implicit-function-declaration")
set(CMAKE_C_FLAGS_RELEASE "-O2 -pipe -fomit-frame-pointer -funroll-loops")
set(CMAKE_C_FLAGS_FAST "-O2")

set(CMAKE_CXX_FLAGS "-std=c++11 ")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -gdwarf-3 -Wall -Werror=implicit -Werror=implicit-int -Werror=implicit-function-declaration")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -pipe -fomit-frame-pointer -funroll-loops")
set(CMAKE_CXX_FLAGS_FAST "-O2")

find_package(Boost 1.54 COMPONENTS thread filesystem regex system date_time program_options log log_setup REQUIRED)

include_directories(
	"${PROJECT_SOURCE_DIR}/Daemon/include/" 
	"${PROJECT_SOURCE_DIR}/Log/include/" 
	"${PROJECT_SOURCE_DIR}/Definitions/include/" 
	"${PROJECT_SOURCE_DIR}/Transport/include/" 
	"${PROJECT_SOURCE_DIR}/Utilities/include/"
	"${PROJECT_SOURCE_DIR}/Processing//include/"
	)

add_subdirectory(Daemon) 
add_subdirectory(Log)
add_subdirectory(Transport)
add_subdirectory(Utilities)
add_subdirectory(Processing)