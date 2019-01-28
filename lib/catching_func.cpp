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
  void func(const bool do_throw_exception) OPTIONAL_EXCEPTION_SPECIFIER
  {
    noexcept_benchmark::throw_exception_if(do_throw_exception);
  }


  void catching_recursive_func(unsigned short number_of_func_calls) OPTIONAL_EXCEPTION_SPECIFIER
  {
    try
    {
      // The compiler cannot assume that this bool is always false, even though it is!
      volatile bool volatile_false = noexcept_benchmark::get_false();

      if (--number_of_func_calls > 0)
      {
        func(volatile_false);
        catching_recursive_func(number_of_func_calls);
      }
    }
    catch (const std::exception&)
    {
      std::cout << number_of_func_calls << std::endl;
    }
  }
}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIBRARY_NAMESPACE::catching_func()
{
  return noexcept_benchmark::profile_func_call([]
  {
    try
    {
      catching_recursive_func(NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS);
    }
    catch (const std::exception&)
    {
      std::cout << __FUNCTION__ << std::endl;
    }
  });
}
