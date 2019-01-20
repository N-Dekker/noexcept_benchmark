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

# Note: VERSION 3.13 would be preferable, as it has IPO (LTCG) for VS.
# However, Travis CI (Ubuntu 4.8.4-2ubuntu1~14.04.3) still runs CMake 3.9.2,
# at the moment (20 January 2019).
cmake_minimum_required( VERSION 3.9.2 )

project (noexcept_benchmark)

add_library(noexcept_lib SHARED
  inline_func_test.cpp)
target_compile_definitions(noexcept_lib PRIVATE SPECIFY_NOEXCEPT=1)

add_library(unspecified_except_lib SHARED
  inline_func_test.cpp)
target_compile_definitions(unspecified_except_lib PRIVATE SPECIFY_NOEXCEPT=0)


add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}_main.cpp)

target_link_libraries(${PROJECT_NAME}
	noexcept_lib
	unspecified_except_lib
	)
