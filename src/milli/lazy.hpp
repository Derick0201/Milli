/*
lazy.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_LIBRARY_LAZY_HPP
#define MILLI_LIBRARY_LAZY_HPP

//todo double check includes
#include <optional>
#include <utility>

namespace milli {

  template<typename T, typename Initializer>
  class lazy {
  public:
    using value_type = T;
    using initializer_type = Initializer;

  private:
    static constexpr bool is_initializer_noexcept =
        noexcept(std::declval<Initializer>()());

  public:

    template <typename ...Args>
    constexpr explicit lazy(std::in_place_t tag, Args&&... value) noexcept(std::is_nothrow_constructible_v<value_type, Args&&...>) : value_(tag, std::forward<Args>(value)...) {}

    constexpr explicit lazy(initializer_type&& init) noexcept(std::is_nothrow_move_constructible_v<initializer_type>) : initializer_(std::forward<initializer_type>(init)) {}

    constexpr lazy(lazy&& rhs) noexcept(std::is_nothrow_move_constructible_v<Initializer> and std::is_nothrow_move_constructible_v<value_type>) = default;

    constexpr lazy(const lazy& rhs) noexcept(std::is_nothrow_constructible_v<Initializer> and std::is_nothrow_constructible_v<value_type>) = default;

    constexpr T& value()& noexcept(is_initializer_noexcept) {
      if (not value_) {
        initialize();
      }

      return value_;
    }

    constexpr const T& value() const& noexcept(is_initializer_noexcept) {
      return static_cast<lazy*>(this)->value();
    }

    constexpr T&& value()&& noexcept(is_initializer_noexcept) {
      return std::move(value());
    }

    template<typename U>
    constexpr T value_or(U&& default_value) const& noexcept(is_initializer_noexcept) {
      if (has_value()) {
        return value();
      }

      return default_value;
    }

    template<typename U>
    constexpr T value_or(U&& default_value) && noexcept(is_initializer_noexcept) {
      if (has_value()) {
        return std::move(value());
      }

      return default_value;
    }

    constexpr const T* operator->() const noexcept(is_initializer_noexcept) {
      return value();
    }

    constexpr T* operator->() noexcept(is_initializer_noexcept) {
      return value();
    }

    constexpr T& operator*()& noexcept(is_initializer_noexcept) {
      return value();
    }

    constexpr const T& operator*() const& noexcept(is_initializer_noexcept) {
      return value();
    }

    constexpr T&& operator*()&& noexcept(is_initializer_noexcept) {
      return value();
    }

    constexpr bool has_value() noexcept {
      return static_cast<bool>(value_);
    }

    constexpr explicit operator bool() noexcept {
      return has_value();
    }

    void initialize() noexcept(is_initializer_noexcept){
      value_.emplace(initializer_());
    }

  private:
    Initializer initializer_;
    //todo add thread safety
     mutable std::optional<T> value_;
  };

}

#if __has_include(<experimental/coroutine>) or __has_include(<coroutine>)

#include "coro/lazy.hpp"

#endif

#endif //MILLI_LIBRARY_LAZY_HPP
