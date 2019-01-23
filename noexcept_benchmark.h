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

#include <cassert>
#include <chrono>
#include <exception>


#ifdef _WIN32
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT __declspec(dllimport)
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT __declspec(dllexport)
#else
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
#endif

#ifdef SPECIFY_NOEXCEPT
#  if SPECIFY_NOEXCEPT == 0
#    define OPTIONAL_EXCEPTION_SPECIFIER
#    define LIBRARY_NAMESPACE implicit_except_test
#  endif
#  if SPECIFY_NOEXCEPT == 1
#    define OPTIONAL_EXCEPTION_SPECIFIER noexcept
#    define LIBRARY_NAMESPACE noexcept_test
#  endif
#endif


// TODO Make these numbers configurable.
#ifdef NDEBUG
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 167890002
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 690000002
#  define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 7500002
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS 10002
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS 1022
#else
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 16789001
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 69000001
#  define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 750001
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS 1001
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS 251
#endif

namespace noexcept_benchmark
{
  inline void throw_if_true(const bool do_throw_exception = false)
  {
    if (do_throw_exception)
    {
      assert(!"This function should only be called with do_throw_exception = false!");
      throw std::exception{};
    }
  }


  template <typename T>
  double profile_func_call(T func)
  {
    using namespace std::chrono;
    
    const auto time_point1 = high_resolution_clock::now();
    func();
    const auto time_point2 = high_resolution_clock::now();

    return duration_cast<duration<double>>(time_point2 - time_point1).count();
  }

}


#endif
