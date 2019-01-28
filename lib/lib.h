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

#ifdef _WIN32
#  ifdef _WINDLL
#    define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT __declspec(dllexport)
#  else
#    define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT __declspec(dllimport)
#  endif
#else
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
#endif



namespace NOEXCEPT_BENCHMARK_LIB_NAMESPACE
{
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT void exported_func(bool do_throw_exception) NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER;
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double catching_func();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_inc_and_dec();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_inline_func();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_vector_reserve();

    class NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT dummy_class
    {
    public:
      dummy_class() NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER;
      ~dummy_class();
    };


    inline void recursive_func(unsigned short number_of_func_calls) NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER
    {
      if (--number_of_func_calls > 0)
      {
        dummy_class dummy1;
        dummy_class dummy2;
        recursive_func(number_of_func_calls);
        dummy_class dummy3;
        dummy_class dummy4;
      }
    }


    template <unsigned number_of_func_calls>
    void recursive_func_template() NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER
    {
      dummy_class dummy1;
      dummy_class dummy2;
      recursive_func_template<number_of_func_calls - 1>();
      dummy_class dummy3;
      dummy_class dummy4;
    }

    template <>
    inline void recursive_func_template<0>() NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER
    {
    }

  }  // End of namespace noexcept_test
