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
#ifndef MILLI_LIBRARY_SRC_MILLI_CORO_LAZY_HPP
#define MILLI_LIBRARY_SRC_MILLI_CORO_LAZY_HPP

#if !__has_include(<experimental/coroutine>) and !__has_include(<coroutine>)
#error "your compiler or standard library implementation does not support coroutines."
#endif

#if __has_include(<experimental/coroutine>)

#include <experimental/coroutine>

namespace milli::coro::detail {
  using suspend_always = std::experimental::suspend_always;
  using suspend_never = std::experimental::suspend_never;
  template<typename T>
  using coroutine_handle = std::experimental::coroutine_handle<T>;
}

#elif __has_include(<coroutine>)
#include <coroutine>

namespace milli::coro::detail{
  using suspend_always = std::suspend_always;
  using suspend_never = std::suspend_never;
  template <typename T>
  using coroutine_handle = std::coroutine_handle<T>;
}
#endif

//todo double check includes
#include <utility>
#include <optional>

namespace milli {

  namespace coro::detail {
    template<typename T>
    struct lazy_promise_type {
      std::optional<T> value;

      auto initial_suspend() { return suspend_always{}; }

      auto final_suspend() { return suspend_always{}; }

      auto unhandled_exception() -> void { throw; }

      auto yield_value() = delete;

      //todo make it ill-formed
      auto await_transform() = delete;

      template<typename U>
      auto return_value(U&& val) -> void { value.emplace(std::forward<U>(val)); }

      auto return_void() = delete;
    };
  }


  template<typename T, typename Promise>
  class lazy<T, std::experimental::coroutine_handle<Promise>> {
  public:
    using value_type = T;
    using coroutine_handle = coro::detail::coroutine_handle<Promise>;

    explicit lazy(coroutine_handle handle) : handle_(handle) {}

    lazy(lazy&& rhs) noexcept = default;

    lazy(const lazy& rhs) = default;


    ~lazy() {
      if (handle_)
        handle_.destroy();
    }

    T& value() & {
      assert(handle_);
      if (not handle_->promise().value) {
        //just in case someone decides to suspend the coroutine
        //todo remove once await_suspend is ill-formed
        while (not handle_->done()) handle_->resume();
      }

      assert(handle_->promise().value);
      return handle_->promise().value;
    }

    const T& value() const& {
      return static_cast<lazy*>(this)->value();
    }

    T&& value() && {
      return std::move(value());
    }

    template<typename U>
    T value_or(U&& default_value) const& {
      if (has_value()) {
        return value();
      }

      return default_value;
    }

    template<typename U>
    T value_or(U&& default_value) && {
      if (has_value()) {
        return std::move(value());
      }

      return default_value;
    }

    const T* operator->() const {
      return value();
    }

    T* operator->() {
      return value();
    }

    T& operator*() & {
      return value();
    }

    const T& operator*() const& {
      return value();
    }

    T&& operator*() && {
      return value();
    }

    bool has_value() {
      return static_cast<bool>(handle_) && static_cast<bool>(handle_.promise().value);
    }

    explicit operator bool() {
      return has_value();
    }

  private:
    std::optional<coroutine_handle> handle_;
  };


}

#endif //MILLI_LIBRARY_SRC_MILLI_CORO_LAZY_H
