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
#include <ctime>
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

#ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS 8
#endif

#ifndef NOEXCEPT_BENCHMARK_THROW_EXCEPTION
#  define NOEXCEPT_BENCHMARK_THROW_EXCEPTION 1
#endif


#ifdef NDEBUG
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 567890000
#  endif
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 98765432
#  endif
#  ifndef NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE
#    define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 987654
#  endif
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS
// Note: On Windows 10, x64, stack overflow occurred with N = 15000
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS 13000
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS 12000
#  endif
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS
// Avoids gcc 4.8.4 error: template instantiation depth exceeds maximum of 900
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS 899
#  endif
#else
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 42
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS 42
#endif


namespace noexcept_benchmark
{
  inline void throw_exception_if(const bool do_throw_exception)
  {
    if (do_throw_exception)
    {
      assert(!"This function should only be called with do_throw_exception = false!");
#if NOEXCEPT_BENCHMARK_THROW_EXCEPTION
      throw std::exception{};
#else

#endif
    }
  }


  inline void throw_exception_if_time_is_zero()
  {
    throw_exception_if(std::time(nullptr) == 0);
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
