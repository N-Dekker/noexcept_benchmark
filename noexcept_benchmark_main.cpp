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
#include <iostream>

namespace noexcept_test
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void test_inline_func();
}

namespace unspecified_exception_specification_test
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void test_inline_func();
}

int main()
{
  std::cout
    << "__FILE__ = " << __FILE__
    << "\nsizeof(void*) = " << sizeof(void*)
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

  for (int numberOfTimes = 0; numberOfTimes < 3; ++numberOfTimes)
  {
    noexcept_test::test_inline_func();
    unspecified_exception_specification_test::test_inline_func();
  }
}