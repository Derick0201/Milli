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

#ifndef MILLI_LIBRARY_NOT_EMPTY_BASE_HPP
#define MILLI_LIBRARY_NOT_EMPTY_BASE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

#ifdef __cpp_lib_optional
#include <optional>
#endif

namespace milli {

  namespace detail {
    struct no_error_handler {
      void operator()(bool condition) noexcept {}
    };
  }

  template<typename T, typename ErrorHandler = detail::no_error_handler>
  class not_empty_base {
  public:
    using stored_type  = T;
    using element_type = typename std::remove_cv<typename std::remove_reference<decltype(*std::declval<stored_type>())>::type>::type;

  private:
    static constexpr bool is_dereference_noexcept = noexcept(*std::declval<stored_type>());
    static constexpr bool is_error_check_noexcept = noexcept(std::declval<ErrorHandler>()(false));

  public:
    not_empty_base() = delete;

    template<typename U, typename = typename std::enable_if<std::is_convertible<U, stored_type>::value>
    ::type>

    constexpr not_empty_base(U&& value) noexcept(std::is_nothrow_constructible<stored_type, decltype((std::forward<U>(
        value)))>::value && is_error_check_noexcept)
        : value_(std::forward<U>(value)) {
      ErrorHandler()(static_cast<bool>(value_));
    }

    not_empty_base(std::nullptr_t) = delete;
    not_empty_base(const std::nullptr_t&) = delete;
    not_empty_base(const std::nullptr_t&&) = delete;

#ifdef __cpp_lib_optional
    not_empty_base(std::nullopt_t) = delete;
    not_empty_base(const std::nullopt_t&) = delete;
    not_empty_base(const std::nullopt_t&&) = delete;
#endif

    constexpr operator const stored_type&() const noexcept {
      return get();
    }

    operator stored_type&() noexcept {
      return get();
    }

    auto operator*() noexcept(is_dereference_noexcept && is_error_check_noexcept) -> element_type& {
      ErrorHandler()(static_cast<bool>(value_));
      return *value_;
    }

    constexpr auto operator*() const noexcept(is_dereference_noexcept && is_error_check_noexcept) -> const element_type& {
      ErrorHandler()(static_cast<bool>(value_));
      return *value_;
    }

    auto operator->() noexcept(is_dereference_noexcept && is_error_check_noexcept) -> element_type* {
      ErrorHandler()(static_cast<bool>(value_));
      return &*(*this);
    }

    constexpr auto operator->() const noexcept(is_dereference_noexcept && is_error_check_noexcept) -> element_type* {
      ErrorHandler()(static_cast<bool>(value_));
      return &*(*this);
    }

    constexpr auto get() const noexcept(is_error_check_noexcept) -> const stored_type& {
      ErrorHandler()(static_cast<bool>(value_));
      return value_;
    }

    constexpr auto get() noexcept(is_error_check_noexcept) -> stored_type& {
      ErrorHandler()(static_cast<bool>(value_));
      return value_;
    }

    template<typename U, typename = typename std::enable_if<
        std::is_convertible<U, stored_type>::value>::type>
    constexpr auto operator=(not_empty_base<U>&& value)
    noexcept(noexcept(std::declval<stored_type&>() = std::forward<U>(value)))
    -> not_empty_base& {
      ErrorHandler()(static_cast<bool>(value_));
      value_ = std::forward<U>(value);
      return *this;
    }

    auto operator=(std::nullptr_t) -> not_empty_base& = delete;
    auto operator=(const std::nullptr_t&) -> not_empty_base& = delete;
    auto operator=(const std::nullptr_t&&) -> not_empty_base& = delete;

#ifdef __cpp_lib_optional
    not_empty_base& operator=(std::nullopt_t) = delete;
    not_empty_base& operator=(const std::nullopt_t&) = delete;
    not_empty_base& operator=(const std::nullopt_t&&) = delete;
#endif

  private:
    stored_type value_;
  };
}

#endif //MILLI_LIBRARY_NOT_EMPTY_BASE_HPP
