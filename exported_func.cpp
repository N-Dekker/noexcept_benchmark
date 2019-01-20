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
#include <exception>

namespace
{
  inline void throw_if_true(const bool do_throw_exception)
  {
    if (do_throw_exception)
    {
      throw std::exception{};
    }
  }
}


namespace LIBRARY_NAMESPACE
{
  NOEXCEPT_BENCHMARK_SHARED_LIB_EXPORT void exported_func(bool do_throw_exception) OPTIONAL_EXCEPTION_SPECIFIER
  {
    assert(!do_throw_exception);
    throw_if_true(do_throw_exception);
  }
}