# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(auth_serial_server)


target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR})

zephyr_include_directories(${CMAKE_SOURCE_DIR}/src)

target_sources(app PRIVATE
  src/main.c
)

#zephyr_library_include_directories(${CMAKE_SOURCE_DIR})
