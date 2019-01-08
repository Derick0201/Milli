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
//todo add support to types with both operators

namespace milli {

  namespace detail {
    template<typename...T>
    struct valid_types {
      using type = void;
    };

#if __cpp_constexpr == 200704L

    template<typename T>
    constexpr auto
    repeat_impl(unsigned long long times, T& callback, unsigned long long current) noexcept(noexcept(callback(0ull))) -> void {
      times == 0 ? void() : times == 1 ? (callback(current)) : (callback(current), repeat_impl(times - 1, callback, current + 1));
    }

    template<typename T>
    constexpr auto
    repeat_impl(unsigned long long times, T& callback) noexcept(noexcept(callback())) -> void {
      times == 0 ? void() : times == 1 ? (callback()) : (callback(), repeat_impl(times - 1, callback));
    }
#endif

  }

#if __cpp_constexpr == 200704L
  template<typename T>
  constexpr auto repeat(unsigned long long times, T callback) noexcept(noexcept(callback(0ull))) -> typename detail::valid_types<
      decltype(std::declval<T>()(std::declval<unsigned long long int>()))>::type {
    detail::repeat_impl(times, callback, 0);
  }

  template<typename T>
  constexpr auto repeat(unsigned long long times, T callback) noexcept(noexcept(callback())) -> typename detail::valid_types<
      decltype(std::declval<T>()())>::type{
    detail::repeat_impl(times, callback);
  }

#elif __cpp_constexpr == 201304L

  template<typename T>
  constexpr auto repeat(unsigned long long times, T callback) noexcept(noexcept(callback(0ull))) -> typename detail::valid_types<
      decltype(std::declval<T>()(std::declval<unsigned long long int>()))>::type  {
    for(unsigned long long i=0; i<times; ++i){
      callback(i);
    }
  }

  template<typename T>
  constexpr auto repeat(unsigned long long times, T callback) noexcept(noexcept(callback())) -> typename detail::valid_types<
      decltype(std::declval<T>()())>::type {
    while(times--)
      callback();
  }

#endif

}

#endif //MILLI_LIBRARY_REPEAT_HPP
