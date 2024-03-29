cmake_minimum_required(VERSION 2.6)
project(MM)

# define libraries
# boost version 1.58 to 1.66
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.58.0 COMPONENTS date_time filesystem regex system thread) 
include_directories(${Boost_INCLUDE_DIRS})

# tbb version 2018 U1
set(TBB_INCLUDE_DIR "/usr/local/include")
set(TBB_LIB_DIR "/usr/local/lib")
find_library(TBB_LIBS tbb HINTS "${TBB_LIB_DIR}")

include_directories(${TBB_INCLUDE_DIR})

# poco version 1.7.9
set(POCO_INCLUDE_DIR "/usr/local/include")
set(POCO_LIB_DIR "/usr/local/lib")
set(POCO_LIBS
	"${POCO_LIB_DIR}/libPocoNet.so"
	"${POCO_LIB_DIR}/libPocoUtil.so"
	"${POCO_LIB_DIR}/libPocoFoundation.so")

include_directories(${POCO_INCLUDE_DIR})

# fmt version 8.0.0 shared build
set(FMT_INCLUDE_DIR "/usr/local/include")
set(FMT_LIB_DIR "/usr/local/lib")
find_library(FMT_LIBS fmt HINTS "${FMT_LIB_DIR}")

include_directories(${FMT_INCLUDE_DIR})

# spdlog header
set(SPDLOG_INCLUDE_DIR "/usr/local/include")

include_directories(${SPDLOG_INCLUDE_DIR})

# moody camel queue header
set(MOODY_CAMEL_INCLUDE_DIR "/usr/local/include")

include_directories(${MOODY_CAMEL_INCLUDE_DIR})

# gtest 1.8.0
set(GTEST_INCLUDE_DIR "/usr/local/include")
set(GTEST_LIB_DIR "/usr/local/lib")
set(GTEST_LIBS
	"${GTEST_LIB_DIR}/libgtest.a"
	"${GTEST_LIB_DIR}/libgtest_main.a"
	"${GTEST_LIB_DIR}/libgmock.a"
	"${GTEST_LIB_DIR}/libgmock_main.a"
)

include_directories(${GTEST_INCLUDE_DIR})

# femas
set(FEMAS_INCLUDE_DIR "/usr/include/femas")
set(FEMAS_LIB_DIR "/usr/lib/femas")
set(FEMAS_LIBS
	"${FEMAS_LIB_DIR}/libUSTPmduserapi.so"
	"${FEMAS_LIB_DIR}/libUSTPtraderapi.so"
)

include_directories(${FEMAS_INCLUDE_DIR})

# compiler options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall -O3 -pipe -pthread -std=c++14 -lboost_thread -lboost_system -fprofile-arcs -ftest-coverage -lssl -lcrypto")
# set(CMAKE_CXX_FLAGS "-g -Wall -pipe -pthread -std=c++14 -lboost_thread -lboost_system -fprofile-arcs -ftest-coverage -lssl -lcrypto")

# testing
set(BUILD_TESTING ON)

# output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/release/bin" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/release/lib" )
set(CMAKE_CONFIG_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/release/config" )
set(CMAKE_COMMAND_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/release/script" )

# project structure
add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(script)
