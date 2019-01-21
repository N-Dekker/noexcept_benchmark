#ifndef noexcept_benchmark_h
#define noexcept_benchmark_h

/*
Copyright Niels Dekker, LKEB, Leiden University Medical Center

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0.txt

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <chrono>
#include <string>


#ifdef _WIN32
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT __declspec(dllimport)
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT __declspec(dllexport)
#else
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
#endif

#if SPECIFY_NOEXCEPT == 0
#define OPTIONAL_EXCEPTION_SPECIFIER
#define LIBRARY_NAMESPACE unspecified_exception_specification_test
const char* const testName = "unspecified-except-spec-test";
#endif
#if SPECIFY_NOEXCEPT == 1
#define OPTIONAL_EXCEPTION_SPECIFIER noexcept
#define LIBRARY_NAMESPACE noexcept_test
const char* const testName = "noexcept-test";
#endif


// TODO Make these numbers configurable.
#ifdef NDEBUG
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 167890002
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 690000002
#  define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 7500002
#else
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 16789001
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 69000001
#  define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 750001
#endif

namespace noexcept_benchmark
{
  inline std::string make_duration_string(
    const std::chrono::high_resolution_clock::time_point time_point1,
    const std::chrono::high_resolution_clock::time_point time_point2)
  {
    const auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(time_point2 - time_point1);
    return "\tDuration = " + std::to_string(duration.count()) + " second(s)\n";
  }
}


#endif
