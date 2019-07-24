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
#include <ctime>

using noexcept_benchmark::throw_exception_if;

namespace
{
  void f(bool b) OPTIONAL_EXCEPTION_SPECIFIER // noexcept or nothing!
  {
    throw_exception_if(b);
  }

}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIB_NAME::test_inc_and_dec()
{
  return noexcept_benchmark::profile_func_call([]
  {
    int v{};
    volatile bool my_volatile_false{ noexcept_benchmark::get_false() };
    try
    {
      for (auto i = NOEXCEPT_BENCHMARK_INC_AND_DEC_FUNC_CALLS; i > 0; --i)
      {
        const bool b{ my_volatile_false };
        ++v;
        f(b);  // b is false, so do not throw
        --v;
      }
    }
    catch (const std::exception&) {}

    if (v != 0) std::cerr << "Error: value = " << v << std::endl;
  });
}
