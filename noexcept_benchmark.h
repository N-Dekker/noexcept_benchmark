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
#include <climits>
#include <exception>


#ifdef SPECIFY_NOEXCEPT
#  if SPECIFY_NOEXCEPT == 0
#    define OPTIONAL_EXCEPTION_SPECIFIER
#    define LIB_NAME implicit_lib
#  endif
#  if SPECIFY_NOEXCEPT == 1
#    define OPTIONAL_EXCEPTION_SPECIFIER noexcept
#    define LIB_NAME noexcept_lib
#  endif
#endif

#ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS 10
#endif

#ifndef NOEXCEPT_BENCHMARK_THROW_EXCEPTION
#  define NOEXCEPT_BENCHMARK_THROW_EXCEPTION 1
#endif


#ifdef NDEBUG
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 2147483647 // INT32_MAX (about two billion)
#  endif
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 200000000 // two hundred million
#  endif
#  ifndef NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS
#    define NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS 10000 // ten thousand
#endif
#  ifndef NOEXCEPT_BENCHMARK_INC_AND_DEC_FUNC_CALLS
#    define NOEXCEPT_BENCHMARK_INC_AND_DEC_FUNC_CALLS 1000000000 // a billion
#  endif
#  ifndef NOEXCEPT_BENCHMARK_STACK_UNWINDING_FUNC_CALLS
// Note: On Windows 10, x64, stack overflow occurred with N = 15000
#    define NOEXCEPT_BENCHMARK_STACK_UNWINDING_FUNC_CALLS 10000 // ten thousand
#  endif
#  ifndef NOEXCEPT_BENCHMARK_STACK_UNWINDING_OBJECTS
// Note: On Windows 10, x64, stack overflow occurred with N = 1280000
#    define NOEXCEPT_BENCHMARK_STACK_UNWINDING_OBJECTS 1000000 // a million
#  endif
#  ifndef NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE
#    define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 10000000 // ten million
#  endif
#else
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_INC_AND_DEC_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_STACK_UNWINDING_FUNC_CALLS 42
#  define NOEXCEPT_BENCHMARK_STACK_UNWINDING_OBJECTS 42
#  define NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE 42
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
#endif
    }
  }


  inline bool get_false()
  {
    // The compiler may not assume that std::time returns non-zero,
    // but in practie, it always does!
    return std::time(nullptr) == 0;
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

#define NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER noexcept
#define NOEXCEPT_BENCHMARK_LIB_NAME noexcept_lib
#include "lib/lib.h"
#undef NOEXCEPT_BENCHMARK_LIB_NAME
#undef NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER
#define NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER
#define NOEXCEPT_BENCHMARK_LIB_NAME implicit_lib
#include "lib/lib.h"
#undef NOEXCEPT_BENCHMARK_LIB_NAME
#undef NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER



#endif
