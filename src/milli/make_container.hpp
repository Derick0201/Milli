/*
make_container.hpp: This file is part of the Milli Library.

    Copyright (C) Dawid Pilarski, PanicSoftware 2018-2019
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


#ifndef MILLI_MOVE_INITIALIZER_LIST_HPP
#define MILLI_MOVE_INITIALIZER_LIST_HPP

#include <functional>
#include <iterator>
#include <initializer_list>
#include <type_traits>

namespace milli{

  namespace detail{

    template <typename... T>
    struct is_same{
      static constexpr bool value = true;
    };

    template <typename T1, typename T2, typename... Ttail>
    struct is_same<T1, T2, Ttail...>{
      static constexpr bool value = is_same<T1, T2>::value && is_same<T2, Ttail...>::value;
    };

    template <typename T1, typename T2>
    struct is_same<T1, T2>{
      static constexpr bool value = std::is_same<T1, T2>::value;
    };

    template <typename T1, typename... Ttail>
    struct first_of_variadic{
      using type = T1;
    };

  }


  template <typename T, typename... Args>
  auto make_container(Args&&... args) -> typename std::enable_if<not std::is_lvalue_reference<typename detail::first_of_variadic<Args...>::type>::value, T>::type{
    using value_type = typename T::value_type;
    using initializer_type = typename detail::first_of_variadic<Args...>::type;
    using value_ref = std::reference_wrapper<initializer_type>;

    static_assert(detail::is_same<Args...>::value, "for make_container all types needs to be the same");
    static_assert(std::is_move_constructible<value_type>::value, "elements for container created with temporary values must be move constructible");

//    std::initializer_list<value_ref> tmp{std::ref(args)...};
    std::array<value_ref, sizeof...(Args)> tmp = {std::ref(args)...};
    auto moveable = [](decltype(tmp.begin()) it){return std::make_move_iterator(it);};
    return {moveable(tmp.begin()), moveable(tmp.end())};
  }

  template <typename T, typename... Args>
  auto make_container(Args&&... args) -> typename std::enable_if<std::is_lvalue_reference<typename detail::first_of_variadic<Args...>::type>::value, T>::type{
    static_assert(detail::is_same<Args...>::value, "for make_container all types needs to be the same");
    using value_type = typename T::value_type;
    static_assert(std::is_copy_constructible<value_type>::value, "elements for container created with temporary values must be copy constructible");

    std::initializer_list<value_type> tmp{args...};
    return {tmp.begin(), tmp.end()};
  }

  template <typename T>
  auto make_container() -> T{
    return {};
  }

}

#endif //MILLI_MOVE_INITIALIZER_LIST_HPP
