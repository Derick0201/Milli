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

  //todo check if common base class is possible
  template<typename T, typename Initializer>
  class lazy {
  public:
    using value_type = T;
    using initializer_type = Initializer;

    //what kind of nothrow here?
    // todo disambiguate constructors
    // todo change T and Initializer info value_type and initializer_type
    constexpr explicit lazy(T&& value) noexcept(std::is_nothrow_constructible<value_type, T&&>::value) : value_(std::forward<T>(value)) {}

    constexpr explicit lazy(Initializer&& init) noexcept(std::is_nothrow_constructible<initializer_type, Initializer&&>::value) : initializer_(std::forward<Initializer>(init)) {}

    constexpr lazy(lazy&& rhs) noexcept(std::is_nothrow_move_constructible<Initializer>::value) = default;

    constexpr lazy(const lazy& rhs) = default;

    constexpr T& value()& {
      if (not value_) {
        value_ = initializer_();
      }

      return value_;
    }

    constexpr const T& value() const& {
      return static_cast<lazy*>(this)->value();
    }

    constexpr T&& value()&& {
      return std::move(value());
    }

    template<typename U>
    constexpr T value_or(U&& default_value) const& {
      if (has_value()) {
        return value();
      }

      return default_value;
    }

    template<typename U>
    constexpr T value_or(U&& default_value)&& {
      if (has_value()) {
        return std::move(value());
      }

      return default_value;
    }

    constexpr const T* operator->() const {
      return value();
    }

    constexpr T* operator->() {
      return value();
    }

    constexpr T& operator*()& {
      return value();
    }

    constexpr const T& operator*() const& {
      return value();
    }

    constexpr T&& operator*()&& {
      return value();
    }

    constexpr bool has_value() {
      return static_cast<bool>(value_);
    }

    constexpr explicit operator bool() {
      return has_value();
    }

  private:
    Initializer initializer_;
    std::optional<T> value_;
  };

}

#if __has_include(<experimental/coroutine>) or __has_include(<coroutine>)

#include "coro/lazy.hpp"

#endif

#endif //MILLI_LIBRARY_LAZY_HPP
