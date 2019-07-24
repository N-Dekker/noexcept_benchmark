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
  void f(const bool do_throw_exception) OPTIONAL_EXCEPTION_SPECIFIER
  {
    noexcept_benchmark::throw_exception_if(do_throw_exception);
  }


  // Profile recursive_func(10'000)
  void recursive_func(std::uint16_t n, volatile bool b) OPTIONAL_EXCEPTION_SPECIFIER
  {
    try {
      if (--n > 0) {
        f(b); // b is false, so f(b) never throws.
        recursive_func(n, b);
      }
    }
    catch (const std::exception&) {
      std::cerr << "Should never occur! n = " << n << std::endl;
    }
  }
}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIB_NAME::catching_func()
{
  return noexcept_benchmark::profile_func_call([]
  {
    recursive_func(NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS, noexcept_benchmark::get_false());
  });
}
