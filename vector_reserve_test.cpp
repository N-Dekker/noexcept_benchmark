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

#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>
#include <cstring>


namespace
{
  class my_string
  {
    char* m_data = nullptr;
  public:
    my_string() = default;
    explicit my_string(std::size_t arg)
      :
      m_data{ (arg == 0) ? nullptr : new char[arg + 1]()}
    {

    }

    my_string(const my_string& arg)
      :
      m_data{ arg.m_data }
    {
      if (m_data != nullptr)
      {
        m_data = std::strcpy(
          new char[std::strlen(m_data) + 1], m_data);
      }
    }

    my_string(my_string&& arg) OPTIONAL_EXCEPTION_SPECIFIER
      :
      m_data{ arg.m_data }
    {
      arg.m_data = nullptr;
    }

    my_string& operator=(const my_string& arg)
    {
      *this = my_string{ arg };
      return *this;
    }

    my_string& operator=(my_string&& arg) OPTIONAL_EXCEPTION_SPECIFIER
    {
      m_data = arg.m_data;
      arg.m_data = nullptr;
      return *this;
    }

    ~my_string()
    {
      delete[] m_data;
    }

  };

}

namespace LIBRARY_NAMESPACE
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT void test_vector_reserve()
  {
    std::vector<my_string> strings(NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE, my_string(1));

    const auto time_point1 = std::chrono::high_resolution_clock::now();

    strings.reserve(strings.capacity() + 1);

    const auto time_point2 = std::chrono::high_resolution_clock::now();

    std::cout << noexcept_benchmark::make_duration_string(time_point1, time_point2) << std::flush;
  }
}
