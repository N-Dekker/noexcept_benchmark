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

namespace
{
  inline void explicitly_terminate_if(const bool do_throw_exception) OPTIONAL_EXCEPTION_SPECIFIER
  {
    try
    {
      noexcept_benchmark::throw_exception_if(do_throw_exception);
    }
    catch (...)
    {
      std::terminate();
    }
  }
}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIB_NAME::test_explicitly_terminate_on_exception()
{
  enum { number_of_func_calls = NOEXCEPT_BENCHMARK_NUMBER_OF_FUNC_CALLS_EXPLICITLY_TERMINATE };

  return noexcept_benchmark::profile_func_call([]
  {
    volatile bool volatile_false{ noexcept_benchmark::get_false() };

    for (int i = 0; i < number_of_func_calls; ++i)
    {
      explicitly_terminate_if(volatile_false);
    }
  });
}
