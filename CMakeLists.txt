cmake_minimum_required(VERSION 3.20)
project(highperf VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

option(HIGHPERF_BUILD_EXAMPLES "Build examples" OFF)
option(HIGHPERF_BUILD_BENCHMARKS "Build benchmarks" OFF)

include(cmake/Platform.cmake)

add_library(highperf_core)
add_library(highperf::core ALIAS highperf_core)
target_sources(highperf_core PRIVATE
  src/base/Timestamp.cpp
  src/base/Buffer.cpp)
target_include_directories(highperf_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(highperf_core PUBLIC cxx_std_20)
target_link_libraries(highperf_core PUBLIC highperf::platform)

enable_testing()
add_subdirectory(tests)
