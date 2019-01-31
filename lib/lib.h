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



namespace NOEXCEPT_BENCHMARK_LIB_NAME
{
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT void exported_func(bool do_throw_exception) NOEXCEPT_BENCHMARK_EXCEPTION_SPECIFIER;
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double catching_func();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_inc_and_dec();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_inline_func();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_stack_unwinding();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_stack_unwinding_array();
    NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT double test_vector_reserve();
}
