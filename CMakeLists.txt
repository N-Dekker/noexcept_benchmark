#=========================================================================
#
#  Copyright Niels Dekker, LKEB, Leiden University Medical Center
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0.txt
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#=========================================================================

cmake_minimum_required( VERSION 3.13 )

project (noexcept_benchmark)

message(STATUS "[${PROJECT_NAME}] CMAKE_VERSION = ${CMAKE_VERSION}")
message(STATUS "[${PROJECT_NAME}] CMAKE_GENERATOR = ${CMAKE_GENERATOR}")

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()

if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Release)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

set(NOEXCEPT_BENCHMARK_THROW_EXCEPTION ON CACHE BOOL "throw_exception_if should throw std::exception{}")

if(NOEXCEPT_BENCHMARK_THROW_EXCEPTION)
  set(NOEXCEPT_BENCHMARK_THROW_EXCEPTION_COMPILE_DEFINITION "NOEXCEPT_BENCHMARK_THROW_EXCEPTION=1")
else()
  set(NOEXCEPT_BENCHMARK_THROW_EXCEPTION_COMPILE_DEFINITION "NOEXCEPT_BENCHMARK_THROW_EXCEPTION=0")
endif()

file(GLOB SHARED_LIB_SOURCE_FILES lib/*.cpp lib/*.h)

add_library(noexcept_lib
  SHARED ${SHARED_LIB_SOURCE_FILES})
target_compile_definitions(noexcept_lib PRIVATE
  ${NOEXCEPT_BENCHMARK_THROW_EXCEPTION_COMPILE_DEFINITION}
  SPECIFY_NOEXCEPT=1
)
target_include_directories(noexcept_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(implicit_lib
  SHARED ${SHARED_LIB_SOURCE_FILES})
target_compile_definitions(implicit_lib PRIVATE
  ${NOEXCEPT_BENCHMARK_THROW_EXCEPTION_COMPILE_DEFINITION}
  SPECIFY_NOEXCEPT=0
)
target_include_directories(implicit_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.h
  ${PROJECT_NAME}_main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${NOEXCEPT_BENCHMARK_THROW_EXCEPTION_COMPILE_DEFINITION}
)

target_link_libraries(${PROJECT_NAME}
  noexcept_lib
  implicit_lib
)
