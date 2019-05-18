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

#include "detail/assert.hpp"
#include <optional>
#include <utility>
#include <functional>

//todo trailing return type
namespace milli {

  template<typename T>
  class lazy {
  public:
    using value_type = T;
    using initializer_type = std::function<value_type()>;
  private:
    static constexpr bool is_initializer_nothrow_constructible = std::is_nothrow_move_constructible_v<initializer_type>;
  public:

    template <typename ...Args>
    constexpr explicit lazy(std::in_place_t tag, Args&&... value) noexcept(std::is_nothrow_constructible_v<value_type, Args&&...>) : optional_value_(tag, std::forward<Args>(value)...) {}

    //todo improve constructor to be explicit about std::function requirements
    constexpr explicit lazy(initializer_type initializer) noexcept(std::is_nothrow_move_constructible_v<initializer_type>) : initializer_(std::move(initializer)) {
      milli_assert(initializer_);
    }

    constexpr lazy(lazy&& rhs) noexcept(is_initializer_nothrow_constructible and std::is_nothrow_move_constructible_v<value_type>) :
    initializer_(std::move(rhs.initializer_)),
    optional_value_(std::move(rhs.optional_value_)){
      rhs.optional_value_.reset();
    }

    constexpr lazy(const lazy& rhs) = delete;

    constexpr T& value()& {
      if (not optional_value_) {
        initialize();
      }

      milli_assert(optional_value_);
      return *optional_value_;
    }

    constexpr T&& value()&& {
      return std::move(value());
    }

    constexpr T* operator->() {
      return &value();
    }

    constexpr T& operator*()& {
      return value();
    }

    constexpr T&& operator*()&& {
      return std::move(value());
    }

    constexpr bool has_value() const noexcept {
      return static_cast<bool>(optional_value_);
    }

    constexpr explicit operator bool() const noexcept {
      return has_value();
    }

    void initialize() {
      optional_value_.emplace(initializer_());
    }

  private:
    initializer_type initializer_;
    std::optional<T> optional_value_;
  };

  template <typename T>
  explicit lazy(std::function<T()>) -> lazy<T>;

  template <typename Initializer>
  explicit lazy(Initializer) -> lazy<decltype(std::declval<Initializer>()())>;
}

#if __has_include(<experimental/coroutine>) or __has_include(<coroutine>)

#include "coro/lazy.hpp"

#endif

#endif //MILLI_LIBRARY_LAZY_HPP
