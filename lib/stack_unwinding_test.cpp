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

namespace
{
  struct object_class
  {
    std::uint16_t & ref;

    ~object_class()
    {
      --ref;
    }
  };

  struct recursion_data
  {
    int number_of_func_calls_to_do;
    volatile bool volatile_false;
    std::uint16_t object_counter;
  };

  void recursive_func(recursion_data& data) OPTIONAL_EXCEPTION_SPECIFIER
  {
    if (--data.number_of_func_calls_to_do > 0)
    {
      noexcept_benchmark::throw_exception_if(data.volatile_false);
      object_class stack_object{ ++data.object_counter };
      recursive_func(data);
    }
  }
}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIB_NAME::test_stack_unwinding()
{
  return noexcept_benchmark::profile_func_call([]
  {
    recursion_data data
    {
      NOEXCEPT_BENCHMARK_STACK_UNWINDING_FUNC_CALLS,
      noexcept_benchmark::get_false(),
      0
    };

    try
    {
      recursive_func(data);
    }
    catch (const std::exception&) { }

    if (data.object_counter != 0)
    {
      std::cerr << "Should never occur! n = " << data.object_counter << '\n';
    }
  });
}
