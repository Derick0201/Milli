/*
not_empty.hpp: This file is part of the Milli Library.

    Copyright (C) Dawid Pilarski, PanicSoftware 2019-2019
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

#ifndef MILLI_LIBRARY_NOT_NULL_HPP
#define MILLI_LIBRARY_NOT_NULL_HPP

#ifndef MILLI_TEST
#include <cassert>
#endif

#include <cstddef>
#include <type_traits>
#include <utility>

#ifdef __cpp_lib_optional
#include <optional>
#endif

namespace milli {

  template<typename T>
  class not_empty {
  public:
    using stored_type  = T;
    using element_type = typename std::remove_cv<typename std::remove_reference<decltype(*std::declval<stored_type>())>::type>::type;

  private:
    static constexpr bool is_dereference_noexcept = noexcept(*std::declval<stored_type>());

  public:
    not_empty() = delete;

    template<typename U, typename = typename std::enable_if<std::is_convertible<U, stored_type>::value>
    ::type>
#if __cpp_constexpr >= 201304
  constexpr
#endif
    not_empty(U&& value) noexcept(std::is_nothrow_constructible<stored_type, decltype((std::forward<U>(value)))>::value)
        : value_(std::forward<U>(value)) {
      assert(static_cast<bool>(value_));
    }

    not_empty(std::nullptr_t) = delete;
    not_empty(const std::nullptr_t&) = delete;
    not_empty(const std::nullptr_t&&) = delete;

#ifdef __cpp_lib_optional
    not_empty(std::nullopt_t) = delete;
    not_empty(const std::nullopt_t&) = delete;
    not_empty(const std::nullopt_t&&) = delete;
#endif

    constexpr operator const stored_type&() const noexcept {
      return get();
    }

    operator stored_type&() noexcept {
      return get();
    }

    auto operator*() noexcept(is_dereference_noexcept) -> element_type& {
#if __cpp_constexpr >= 201304
      assert(static_cast<bool>(value_));
#endif
      return *value_;
    }

    constexpr auto operator*() const noexcept(is_dereference_noexcept) -> const element_type& {
#if __cpp_constexpr >= 201304
      assert(static_cast<bool>(value_));
#endif
      return *value_;
    }

    auto operator->() noexcept(is_dereference_noexcept) -> element_type* {
#if __cpp_constexpr >= 201304
      assert(static_cast<bool>(value_));
#endif
      return &*(*this);
    }

    constexpr auto operator->() const noexcept(is_dereference_noexcept) -> element_type* {
#if __cpp_constexpr >= 201304
      assert(static_cast<bool>(value_));
#endif
      return &*(*this);
    }

    constexpr auto get() const noexcept -> const stored_type& {
#if __cpp_constexpr >= 201304
      assert(static_cast<bool>(value_));
#endif
      return value_;
    }

    auto get() noexcept -> stored_type& {
#if __cpp_constexpr >= 201304
      assert(static_cast<bool>(value_));
#endif
      return value_;
    }

    template<typename U, typename = typename std::enable_if<
        std::is_convertible<U, stored_type>::value>::type>
    constexpr auto operator=(not_empty<U>&& value)
    noexcept(noexcept(std::declval<stored_type&>() = std::forward<U>(value)))
    -> not_empty& {
      assert(static_cast<bool>(value_));
      value_ = std::forward<U>(value);
      return *this;
    }

    auto operator=(std::nullptr_t) -> not_empty& = delete;
    auto operator=(const std::nullptr_t&) -> not_empty& = delete;
    auto operator=(const std::nullptr_t&&) -> not_empty& = delete;

#ifdef __cpp_lib_optional
    not_empty& operator=(std::nullopt_t) = delete;
    not_empty& operator=(const std::nullopt_t&) = delete;
    not_empty& operator=(const std::nullopt_t&&) = delete;
#endif

  private:
    stored_type value_;
  };

#ifdef __cpp_deduction_guides

  template<typename T>
  not_empty(T) -> not_empty<T>;

#endif


}

#endif //MILLI_LIBRARY_NOT_NULL_HPP
