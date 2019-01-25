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
#include <algorithm>
#include <chrono>
#include <limits>
#include <iostream>
#include <string>

using namespace noexcept_benchmark;

namespace noexcept_test
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void exported_func(bool do_throw_exception) noexcept;
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT double test_inline_func();
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT double test_vector_reserve();

  class NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT dummy_class
  {
  public:
    dummy_class() noexcept;
    ~dummy_class();
  };


  void recursive_func(unsigned short numberOfFuncCalls) noexcept
  {
    dummy_class dummy;

    if (--numberOfFuncCalls > 0)
    {
      recursive_func(numberOfFuncCalls);
    }
  }

  template <unsigned numberOfFuncCalls>
  void recursive_func_template() noexcept
  {
    dummy_class dummy;
    recursive_func_template<numberOfFuncCalls - 1>();
  }

  template <>
  void recursive_func_template<0>() noexcept
  {
  }

}

namespace implicit_except_test
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT void exported_func(bool do_throw_exception);
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT double test_inline_func();
  NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT double test_vector_reserve();

  class NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT dummy_class
  {
  public:
    dummy_class(); // No noexcept
    ~dummy_class();
  };

  template <unsigned numberOfFuncCalls>
  void recursive_func_template()
  {
    dummy_class dummy;
    recursive_func_template<numberOfFuncCalls - 1>();
  }

  template <>
  void recursive_func_template<0>()
  {
  }


  void recursive_func(unsigned short numberOfFuncCalls)
  {
    dummy_class dummy;

    if (--numberOfFuncCalls > 0)
    {
      recursive_func(numberOfFuncCalls);
    }
  }
}

namespace
{
  const int number_of_iterations = NUMBER_OF_ITERATIONS;

  struct durations_type
  {
    double duration_noexcept;
    double duration_implicit;
  };

  struct test_result
  {
    unsigned number_of_times_noexcept_is_faster = 0;
    unsigned number_of_times_implicit_is_faster = 0;
    double sum_of_durations_noexcept = 0.0;
    double sum_of_durations_implicit = 0.0;
  };


  template <typename T1, typename T2>
  durations_type profile_func_calls(T1 func1, T2 func2)
  {
    durations_type durations;
    durations.duration_noexcept = profile_func_call(func1);
    durations.duration_implicit = profile_func_call(func2);
    return durations;
  }
    

  std::string duration_seconds_to_string(const double seconds)
  {
    return "\tDuration = " + std::to_string(seconds) + " sec. ";
  }
  
  
  void print_durations(const durations_type& durations)
  {
    std::cout
      << duration_seconds_to_string(durations.duration_noexcept)
      << "(explicitly defined 'noexcept')\n"
      << duration_seconds_to_string(durations.duration_implicit)
      << "(implicitly defined exception specification)\n"
      << std::flush;
  }


  void update_test_result(test_result& result, const durations_type& durations)
  {
    result.sum_of_durations_noexcept += durations.duration_noexcept;
    result.sum_of_durations_implicit += durations.duration_implicit;

    if (durations.duration_noexcept < durations.duration_implicit)
    {
      ++result.number_of_times_noexcept_is_faster;
    }
    if (durations.duration_implicit < durations.duration_noexcept)
    {
      ++result.number_of_times_implicit_is_faster;
    }
  }

  double divide_by_positive(const double x, const double y)
  {
    return x /
      ((y > 0) ? y : std::numeric_limits<double>::denorm_min());
  }


  void print_conclusion(const test_result& result)
  {
    if (result.number_of_times_noexcept_is_faster == number_of_iterations)
    {
      std::cout << "So 'noexcept' seems approximately "
        << divide_by_positive(result.sum_of_durations_implicit, result.sum_of_durations_noexcept)
        << " x faster, for this test case.";
    }
    else
    {
      if (result.number_of_times_implicit_is_faster == number_of_iterations)
      {
        std::cout
          << "So an implicitly defined exception specification seems approximately "
          << divide_by_positive(result.sum_of_durations_noexcept, result.sum_of_durations_implicit)
          << " x faster, for this test case.";
      }
      else
      {
        std::cout << "So it seems unclear whether 'noexcept' or implicit is faster, for this test case.";
      }
    }
    std::cout << std::endl;
  }

}

int main()
{
  std::cout
    << "__FILE__ = " << __FILE__
    << "\nsizeof(void*) = " << sizeof(void*)
    << "\n__DATE__ = " << __DATE__
    << "\n__TIME__ = " << __TIME__
#ifdef __VERSION__
    << "\n__VERSION__ = "
    __VERSION__
#endif
#ifdef _MSC_FULL_VER
    << "\n_MSC_FULL_VER = "
    << _MSC_FULL_VER
#endif
#ifdef _MSC_BUILD
    << "\n_MSC_BUILD = "
    << _MSC_BUILD
#endif
#ifdef _DEBUG
    << "\n_DEBUG"
#endif
#ifdef NDEBUG
    << "\nNDEBUG (\"Not Debug\")"
#endif
    << "\nNOEXCEPT_BENCHMARK_THROW_EXCEPTION = "
    << NOEXCEPT_BENCHMARK_THROW_EXCEPTION
    << std::endl;


  {
    test_result result;

    std::cout << "\n[inline function calls (N = "
      << NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS
      << ")]"
      << std::endl;

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::test_inline_func();
      durations.duration_implicit = implicit_except_test::test_inline_func();
      print_durations(durations);
      update_test_result(result, durations);
    }
    print_conclusion(result);
  }
  {
    test_result result;

    std::cout << "\n[recursive function template calls (N = "
      << NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS
      << ")]"
      << std::endl;

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      enum { numberOfFuncCalls = NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_TEMPLATE_CALLS };

      const auto durations = profile_func_calls(
        []
      {
        noexcept_test::recursive_func_template<numberOfFuncCalls>();
      },
        []
      {
        implicit_except_test::recursive_func_template<numberOfFuncCalls>();;
      });

      print_durations(durations);
      update_test_result(result, durations);
    }
    print_conclusion(result);
  }
  {
    test_result result;

    std::cout << "\n[recursive function calls (N = "
      << NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS
      << ")]"
      << std::endl;

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      enum { numberOfFuncCalls = NOEXCEPT_BENCHMARK_NUMBER_OF_RECURSIVE_FUNC_CALLS };

      const auto durations = profile_func_calls(
        []
      {
        noexcept_test::recursive_func(numberOfFuncCalls);
      },
        []
      {
        implicit_except_test::recursive_func(numberOfFuncCalls);
      });

      print_durations(durations);
      update_test_result(result, durations);
    }
    print_conclusion(result);
  }
  {
    test_result result;

    std::cout << "\n[exported library function calls (N = "
      << NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS
      << ")]"
      << std::endl;

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      enum { numberOfFuncCalls = NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS };

      const auto durations = profile_func_calls(
        []
      {
        for (int i = 0; i < numberOfFuncCalls; ++i)
        {
          noexcept_test::exported_func(false);
        }
      },
        []
      {
        for (int i = 0; i < numberOfFuncCalls; ++i)
        {
          implicit_except_test::exported_func(false);
        }
      });
      print_durations(durations);
      update_test_result(result, durations);
    }
    print_conclusion(result);
  }
  {
    test_result result;

    std::cout << "\n[std::vector reserve (N = "
      << NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE
      << ")]"
      << std::endl;

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::test_vector_reserve();
      durations.duration_implicit = implicit_except_test::test_vector_reserve();
      print_durations(durations);
      update_test_result(result, durations);
    }
    print_conclusion(result);
  }
  return 0;
}