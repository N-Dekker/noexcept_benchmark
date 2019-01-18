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

#include <cassert>
#include <chrono>
#include <iostream>
#include <exception>


#if SPECIFY_NOEXCEPT == 0
#define OPTIONAL_EXCEPTION_SPECIFIER
#define LIBRARY_NAMESPACE unspecified_exception_specification_test
constexpr const char* testName = "unspecified-except-spec-test";
#endif
#if SPECIFY_NOEXCEPT == 1
#define OPTIONAL_EXCEPTION_SPECIFIER noexcept
#define LIBRARY_NAMESPACE noexcept_test
constexpr const char* testName = "noexcept-test";
#endif


namespace
{
  inline void throw_if_true(const bool do_throw_exception)
  {
    if (do_throw_exception)
    {
      throw std::exception{};
    }
  }


  inline void inline_func(bool do_throw_exception) OPTIONAL_EXCEPTION_SPECIFIER
  {
    assert(!do_throw_exception);
    throw_if_true(do_throw_exception);
  }
}


namespace LIBRARY_NAMESPACE
{
  __declspec(dllexport) void test_inline_func()
  {
    using namespace std::chrono;
    const auto time_before_func_calls = high_resolution_clock::now();

    constexpr int number_of_func_calls = 1678900000;

    for (int i = 0; i < number_of_func_calls; ++i)
    {
      inline_func(false);
    }

    const auto time_after_func_calls = high_resolution_clock::now();

    const auto durationSeconds =
      duration_cast<duration<double>>(time_after_func_calls - time_before_func_calls);

    std::cout << " Duration " << testName << ": \t" << durationSeconds.count() << " second(s)" << std::endl;
  }
}
