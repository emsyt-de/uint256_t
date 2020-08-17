project(uint256_t LANGUAGES CXX)

cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_library(UInt256)

target_include_directories(UInt256 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(UInt256 PRIVATE
	uint256_t.h
	uint256_t.cpp
	)

add_subdirectory(tests)
