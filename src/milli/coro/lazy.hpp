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
#include <optional>
#include <utility>

namespace milli::coro {

  namespace detail {
    template<typename T>
    struct lazy_promise_type_base {
      std::optional<T> value;

      auto initial_suspend() { return suspend_always{}; }

      auto final_suspend() { return suspend_always{}; }

      auto unhandled_exception() -> void { throw; }

      auto yield_value() = delete;

      //todo make it ill-formed. Or is it now?
      auto await_transform() = delete;

      template<typename U>
      auto return_value(U&& val) -> void { value.emplace(std::forward<U>(val)); }
    };
  }

  //todo merge into milli::lazy otherwise this makes no sense.
  template<typename T>
  class lazy {
  public:
    struct promise_type : detail::lazy_promise_type_base<T> {
      lazy get_return_object() {
        return lazy{detail::coroutine_handle<promise_type>::from_promise(*this)};
      }
    };

    using value_type = T;
    using coroutine_handle = coro::detail::coroutine_handle<promise_type>;

    explicit lazy(coroutine_handle handle) : handle_(handle) {}

    lazy(lazy&& rhs) noexcept : handle_(std::move(rhs.handle_)) {
      rhs.handle_.reset();
    }

    // create shared_lazy for that
    lazy(const lazy& rhs) = delete;

    ~lazy() {
      if (handle_)
        handle_->destroy();
    }

    T& value() const& {
      assert(handle_);
      if (not handle_->promise().value) {
        //just in case someone decides to suspend the coroutine
        //todo remove once await_suspend is ill-formed
        while (not handle_->done()) handle_->resume();
      }

      assert(handle_->promise().value);
      return handle_->promise().value;
    }

    T&& value()&& {
      return std::move(value());
    }

    template<typename U>
    T value_or(U&& default_value) const& noexcept {
      if (has_value()) {
        return value();
      }

      return default_value;
    }

    template<typename U>
    T value_or(U&& default_value)&& noexcept {
      if (has_value()) {
        return std::move(value());
      }

      return default_value;
    }

    T const* operator->() const {
      return value();
    }

    T* operator->() {
      return value();
    }

    T& operator*()& {
      return value();
    }

    T const& operator*() const& {
      return value();
    }

    T&& operator*()&& {
      return value();
    }

    bool has_value() const {
      return static_cast<bool>(handle_) && static_cast<bool>(handle_->promise().value);
    }

    explicit operator bool() const {
      return has_value();
    }

  private:
    //todo add thread safety
    mutable std::optional<coroutine_handle> handle_;
  };


}

#endif //MILLI_LIBRARY_SRC_MILLI_CORO_LAZY_H
