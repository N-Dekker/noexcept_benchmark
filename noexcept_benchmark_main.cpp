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

#include "noexcept_benchmark.h"
#include <chrono>
#include <iostream>

namespace noexcept_test
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void exported_func(bool do_throw_exception) noexcept;
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void test_inline_func();
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void test_vector_reserve();

  class NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT dummy_class
  {
  public:
    dummy_class() noexcept;
    ~dummy_class();
  };
}

namespace unspecified_exception_specification_test
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void exported_func(bool do_throw_exception);
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void test_inline_func();
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void test_vector_reserve();

  class NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT dummy_class
  {
  public:
    dummy_class(); // No noexcept
    ~dummy_class();
  };
}

namespace
{
  template <typename T>
  void recursive_func(unsigned short numberOfFuncCalls)
  {
    T dummy;

    if (--numberOfFuncCalls > 0)
    {
      recursive_func<T>(numberOfFuncCalls);
    }
  }
}

int main()
{
  std::cout
    << "__FILE__ = " << __FILE__
    << "\nsizeof(void*) = " << sizeof(void*)
#ifdef __VERSION__
    << "\n__VERSION__ = "
    __VERSION__
#endif
#ifdef _MSC_FULL_VER
    << "\n_MSC_FULL_VER = "
    << _MSC_FULL_VER
#endif
#ifdef _MSC_BUILD
    << "\n_MSC_BUILD = "
    << _MSC_BUILD
#endif
#ifdef _DEBUG
    << "\n_DEBUG"
#endif
#ifdef NDEBUG
    << "\nNDEBUG (\"Not Debug\")"
#endif
    << std::endl;

  std::cout << "\n[test_inline_func (N = "
    << NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS
    << ")]"
    << std::endl;

  for (int numberOfTimes = 0; numberOfTimes < 3; ++numberOfTimes)
  {
    noexcept_test::test_inline_func();
    unspecified_exception_specification_test::test_inline_func();
  }

  std::cout << "\n[recursive_func (N = "
    << NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS
    << ")]"
    << std::endl;

  for (int numberOfTimes = 0; numberOfTimes < 3; ++numberOfTimes)
  {
    enum { numberOfFuncCalls = NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS };

    std::cout
      << "noexcept-test:\n"
      << noexcept_benchmark::profile_func_call([]
    {
      recursive_func<noexcept_test::dummy_class>(numberOfFuncCalls);
    })
      << "unspecified-except-spec-test:\n"
      << noexcept_benchmark::profile_func_call([]
    {
      recursive_func<unspecified_exception_specification_test::dummy_class>(numberOfFuncCalls);
    })
      << std::flush;
  }

  std::cout << "\n[exported_func(false) calls (N = "
    << NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS
    << ")]"
    << std::endl;

  for (int numberOfTimes = 0; numberOfTimes < 3; ++numberOfTimes)
  {
    enum { numberOfFuncCalls = NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS };

    std::cout
      << "noexcept-test:\n"
      << noexcept_benchmark::profile_func_call([]
    {
      for (int i = 0; i < numberOfFuncCalls; ++i)
      {
        noexcept_test::exported_func(false);
      }
    })
      << "unspecified-except-spec-test:\n"
      << noexcept_benchmark::profile_func_call([]
    {
      for (int i = 0; i < numberOfFuncCalls; ++i)
      {
        unspecified_exception_specification_test::exported_func(false);
      }
    })
      << std::flush;
  }
  std::cout << "\n[test_vector_reserve (N = "
    << NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE
    << ")]"
    << std::endl;

  for (int numberOfTimes = 0; numberOfTimes < 3; ++numberOfTimes)
  {
    std::cout << "noexcept-test:\n" << std::flush;
    noexcept_test::test_vector_reserve();
    std::cout << "unspecified-except-spec-test:\n" << std::flush;
    unspecified_exception_specification_test::test_vector_reserve();
  }

}