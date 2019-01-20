#ifndef noexcept_benchmark_h
#define noexcept_benchmark_h

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
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT __declspec(dllimport)
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT __declspec(dllexport)
#else
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_IMPORT
#  define NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT
#endif

// TODO Make this number configurable.
#ifdef NDEBUG
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 167890002
#else
#  define NOEXCEPT_BENCHMARK_NUMBER_OF_INLINE_FUNC_CALLS 16789001
#endif

#endif