# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(central_auth)


message("** dir is: ${CMAKE_SOURCE_DIR}" )

target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR})

target_sources(app PRIVATE
  src/main.c
)

#zephyr_library_include_directories(${CMAKE_SOURCE_DIR})
