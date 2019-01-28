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
#include <climits>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace noexcept_benchmark;



namespace
{
  const int number_of_iterations = NOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS;
  const std::streamsize output_precision = 8;

  const unsigned column_gap_size = 2;
  const std::string column_gap(column_gap_size, ' ');
  const std::string indent(4, ' ');

  struct durations_type
  {
    double duration_noexcept;
    double duration_implicit;
  };

  
  double divide_by_positive(const double x, const double y)
  {
    return x /
      ((y > 0) ? y : std::numeric_limits<double>::denorm_min());
  }

  char get_comparison_char(const double x, const double y)
  {
    return (x < y) ? '<' : (x > y) ? '>' : (x == y) ? '=' : ' ';
  }

  template <unsigned N>
  class test_result
  {
    unsigned m_number_of_times_noexcept_is_faster = 0;
    unsigned m_number_of_times_implicit_is_faster = 0;
    double m_sum_of_durations_noexcept = 0.0;
    double m_sum_of_durations_implicit = 0.0;
    double m_shortest_duration_noexcept = std::numeric_limits<double>::infinity();
    double m_shortest_duration_implicit = std::numeric_limits<double>::infinity();
    const char* const m_test_case_name;

  public:

    explicit test_result(const char* const test_case_name)
      :
      m_test_case_name{ test_case_name }
    {
      std::cout 
        << "\n"
        << "["
#ifdef _MSC_VER
        << "MSVC "
        << (CHAR_BIT * sizeof(void*))
        << "-bit"
#endif
#ifdef __linux__
        << "Linux "
#endif
#ifdef __APPLE__
        << "Apple "
#endif
#ifdef __clang__
        << "Clang"
#elif defined(__GNUC__)
        << "GCC"
#endif
        << "]"
#if NOEXCEPT_BENCHMARK_THROW_EXCEPTION
        << "[`throw` included]"
#else
        << "[`throw` excluded]"
#endif
        << "[" << test_case_name << " (N = " << N
        << ")]\n"
        << indent
        << "noexcept"
        << std::string(output_precision + 3 - sizeof("noexcept"), ' ')
        << std::string(2 * column_gap_size + 1, ' ')
        << "implicit"
        << std::string(output_precision + 3 - sizeof("implicit"), ' ')
        << column_gap
        << "(durations in seconds)"
        << std::flush;
    }

    void update_test_result(const durations_type& durations)
    {
      m_sum_of_durations_noexcept += durations.duration_noexcept;
      m_sum_of_durations_implicit += durations.duration_implicit;

      m_shortest_duration_noexcept = std::min(m_shortest_duration_noexcept, durations.duration_noexcept);
      m_shortest_duration_implicit = std::min(m_shortest_duration_implicit, durations.duration_implicit);

      if (durations.duration_noexcept < durations.duration_implicit)
      {
        ++m_number_of_times_noexcept_is_faster;
      }
      if (durations.duration_implicit < durations.duration_noexcept)
      {
        ++m_number_of_times_implicit_is_faster;
      }
    }

    ~test_result()
    {
      const std::string dashes(output_precision + 2, '-');

      std::cout
        << '\n'
        << indent
        << dashes
        << std::string(2 * column_gap_size + 1, ' ')
        << dashes
        << '\n'
        << indent
        << m_sum_of_durations_noexcept
        << column_gap
        << get_comparison_char(m_sum_of_durations_noexcept, m_sum_of_durations_implicit)
        << column_gap
        << m_sum_of_durations_implicit
        << column_gap
        << "(sum of durations)\n"
        << indent
        << m_shortest_duration_noexcept
        << column_gap
        << get_comparison_char(m_shortest_duration_noexcept, m_shortest_duration_implicit)
        << column_gap
        << m_shortest_duration_implicit
        << column_gap
        << "(shortest durations)"
        << "\nRatio sum of durations noexcept/implicit: "
        << divide_by_positive(m_sum_of_durations_noexcept, m_sum_of_durations_implicit)
        << "\nRatio sum of durations implicit/noexcept: "
        << divide_by_positive(m_sum_of_durations_implicit, m_sum_of_durations_noexcept)
        << ((m_number_of_times_noexcept_is_faster == number_of_iterations) ?
          "\nIn this case, 'noexcept' specifications always appear faster." : "")
        << ((m_number_of_times_implicit_is_faster == number_of_iterations) ?
          "\nIn this case, implicit exception specifications always appear faster." : "")
        << (((m_number_of_times_noexcept_is_faster < number_of_iterations) &&
            (m_number_of_times_implicit_is_faster < number_of_iterations)) ?
          "\nIn this case, neither implicit nor noexcept specifications always appear faster." : "")
        << std::endl;
    }
  };


  template <typename T1, typename T2>
  durations_type profile_func_calls(T1 func1, T2 func2)
  {
    durations_type durations;
    durations.duration_noexcept = profile_func_call(func1);
    durations.duration_implicit = profile_func_call(func2);
    return durations;
  }
    

  template <unsigned N>
  void update_test_result_and_print_durations(
    test_result<N>& result,
    const durations_type& durations)
  {
    result.update_test_result(durations);
    std::cout
      << '\n'
      << indent
      << durations.duration_noexcept
      << column_gap
      << get_comparison_char(durations.duration_noexcept, durations.duration_implicit)
      << column_gap
      << durations.duration_implicit
      << std::flush;
  }

}

int main()
{
  std::cout
    << std::fixed
    << std::setprecision(output_precision)
    << "The noexcept benchmark from https://github.com/N-Dekker/noexcept_benchmark"
    << "\n__FILE__ = " << __FILE__
    << "\nsizeof(void*) = " << sizeof(void*)
    << " (" << CHAR_BIT * sizeof(void*) << "-bit)"
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
    << "\nNOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS = "
    << NOEXCEPT_BENCHMARK_NUMBER_OF_ITERATIONS
    << "\nNOEXCEPT_BENCHMARK_THROW_EXCEPTION = "
    << NOEXCEPT_BENCHMARK_THROW_EXCEPTION
#if NOEXCEPT_BENCHMARK_THROW_EXCEPTION
    << " (`throw exception{}` included by #if)"
#else
    << " (`throw exception{}` excluded by #if)"
#endif
    << std::endl;
  {
    test_result<NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS> result(
      "inline function calls");

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::test_inline_func();
      durations.duration_implicit = implicit_test::test_inline_func();
      update_test_result_and_print_durations(result, durations);
    }
  }
  {
    test_result<NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS> result(
      "exported library function calls");

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      enum { number_of_func_calls = NOEXCEPT_BENCHMARK_NUMBER_OF_EXPORTED_FUNC_CALLS };

      update_test_result_and_print_durations(result, profile_func_calls(
        []
      {
        volatile bool do_throw_exception = noexcept_benchmark::get_false();

        for (int i = 0; i < number_of_func_calls; ++i)
        {
          noexcept_test::exported_func(do_throw_exception);
        }
      },
        []
      {
        volatile bool do_throw_exception = noexcept_benchmark::get_false();

        for (int i = 0; i < number_of_func_calls; ++i)
        {
          implicit_test::exported_func(do_throw_exception);
        }
      }));
    }
  }
  {
    test_result<NOEXCEPT_BENCHMARK_NUMBER_OF_CATCHING_RECURSIVE_FUNC_CALLS> result(
      "catching function calls");

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::catching_func();
      durations.duration_implicit = implicit_test::catching_func();
      update_test_result_and_print_durations(result, durations);
    }
  }
  {
    test_result<NOEXCEPT_BENCHMARK_INC_AND_DEC_FUNC_CALLS> result(
      "inc `++` and dec `--`");

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::test_inc_and_dec();
      durations.duration_implicit = implicit_test::test_inc_and_dec();
      update_test_result_and_print_durations(result, durations);
    }
  }
  {
    test_result<NOEXCEPT_BENCHMARK_STACK_UNWINDING_FUNC_CALLS> result(
      "stack unwinding");

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::test_stack_unwinding();
      durations.duration_implicit = implicit_test::test_stack_unwinding();
      update_test_result_and_print_durations(result, durations);
    }
  }
  {
    test_result<NOEXCEPT_BENCHMARK_INITIAL_VECTOR_SIZE> result(
      "std::vector<my_string> reserve");

    for (int iteration_number = 0; iteration_number < number_of_iterations; ++iteration_number)
    {
      durations_type durations;
      durations.duration_noexcept = noexcept_test::test_vector_reserve();
      durations.duration_implicit = implicit_test::test_vector_reserve();
      update_test_result_and_print_durations(result, durations);
    }
  }

  std::cout << std::string(80, '=') << std::endl;
  return 0;
}
