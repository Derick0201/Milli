/*
repeat.hpp: This file is part of the Milli Library.

    Copyright (C) Dawid Pilarski, PanicSoftware 2009-2010
    Distributed under the BSD 3-clause License.
    (See Milli/LICENSE)

    Milli IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#ifndef MILLI_LIBRARY_REPEAT_HPP
#define MILLI_LIBRARY_REPEAT_HPP

#include <type_traits>
#include <utility>

namespace milli {

  namespace detail {

    template<typename...T>
    struct valid_types {
      using type = void;
    };

  }

  template<typename T>
  auto repeat(unsigned long long times, T&& callback) noexcept(noexcept(callback(0ull))) -> typename detail::valid_types<
      decltype(std::declval<T>()(std::declval<unsigned long long int>()))>::type {
    for (unsigned long long i = 0; i < times; ++i) {
      callback(i);
    }
  }

  template<typename T>
  auto repeat(unsigned long long times, T&& callback) noexcept(noexcept(callback())) -> typename detail::valid_types<
      decltype(std::declval<T>()())>::type {
    while (times--)
      callback();
  }

}

#endif //MILLI_LIBRARY_REPEAT_HPP
