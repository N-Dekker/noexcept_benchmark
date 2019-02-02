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
  class object_class
  {
    static unsigned m_object_counter;
  public:
    object_class()
    {
      ++m_object_counter;
      volatile bool volatile_false = false;
      noexcept_benchmark::throw_exception_if(volatile_false);
    }

    ~object_class()
    {
      --m_object_counter;
    }

    static unsigned get_object_counter()
    {
      return m_object_counter;
    }
  };

  
  unsigned object_class::m_object_counter;


  void func() OPTIONAL_EXCEPTION_SPECIFIER
  {
    object_class arr[NOEXCEPT_BENCHMARK_STACK_UNWINDING_OBJECTS];
  }
}


NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
double LIB_NAME::test_stack_unwinding_array()
{
  return noexcept_benchmark::profile_func_call([]
  {
    try
    {
      func();
    }
    catch (const std::exception&)
    {
      // Should never occur!
      std::cerr << "Error! object_counter = " << object_class::get_object_counter() << std::endl;
    }
  });
}
