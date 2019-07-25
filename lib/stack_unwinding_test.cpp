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
  struct ref_holder_type {
    std::uint16_t & ref;
    ~ref_holder_type() { --ref; }
  };

  struct recursion_test {
    std::uint16_t num_func_calls_to_do{ NOEXCEPT_BENCHMARK_STACK_UNWINDING_FUNC_CALLS };
    std::uint16_t num_objects_on_stack{ 0 };
    volatile bool b{ noexcept_benchmark::get_false() };

    void recursive_mem_func() OPTIONAL_EXCEPTION_SPECIFIER
    {
      if (--num_func_calls_to_do > 0) {
        noexcept_benchmark::throw_exception_if(b);
        ref_holder_type ref_holder_object{ ++num_objects_on_stack };
        this->recursive_mem_func();
      }
    }
  };

}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIB_NAME::test_stack_unwinding()
{
  return noexcept_benchmark::profile_func_call([]
  {
    recursion_test test;

    try
    {
      test.recursive_mem_func();
    }
    catch (const std::exception&) { }

    if (test.num_objects_on_stack != 0)
    {
      std::cerr << "Should never occur! num_objects_on_stack = " << test.num_objects_on_stack << '\n';
    }
  });
}
