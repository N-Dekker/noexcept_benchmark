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

namespace
{
  void func() OPTIONAL_EXCEPTION_SPECIFIER
  {
    noexcept_benchmark::throw_exception_if_time_is_zero();
  }
}


namespace LIBRARY_NAMESPACE
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT void test_inc_and_dec() OPTIONAL_EXCEPTION_SPECIFIER
  {
    const int number_of_func_calls = NOEXCEPT_BENCHMARK_INC_AND_DEC_FUNC_CALLS;
    int value = 0;

    try
    {
      for (int i = 0; i < number_of_func_calls; ++i)
      {
        ++value;
        func();
        --value;
      }
    }
    catch (const std::exception&)
    {
    }
    if (value != 0)
    {
      std::cout << value << std::endl;
    }
  }
}
