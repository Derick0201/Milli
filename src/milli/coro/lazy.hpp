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


#include <milli/coro/detail/has_coro_support.hpp>
#include <milli/lazy.hpp> //for IDE's convenience
#include <optional>
#include <utility>
#include <functional>

#ifndef MILLI_HAS_CORO_SUPPORT
#error "your compiler or standard library does not support coroutines"
#endif

namespace milli::coro {

  namespace detail {
    template<typename T>
    struct lazy_promise_type {
      std::optional<T> value;

      lazy<T> get_return_object(){
        auto this_coro = detail::coroutine_handle<lazy_promise_type>::from_promise(*this);
        auto initializer = [this_coro, this] () mutable {
          while(not this_coro.done())
            this_coro.resume();

          milli_assert(this->value);
          auto value = *(this->value);
          this_coro.destroy();
          return value;
        };

        std::function<T()> type_erased_initializer = initializer;
        return lazy{type_erased_initializer};
      }

      auto initial_suspend() { return suspend_always{}; }

      auto final_suspend() { return suspend_always{}; }

      auto unhandled_exception() -> void { throw; }

      auto yield_value() = delete;

      auto await_transform() = delete;

      template<typename U>
      auto return_value(U&& val) -> void { value.emplace(std::forward<U>(val)); }
    };
  }
}

#if __has_include(<experimental/coroutine>)
namespace std::experimental {
  template <typename T>
  struct coroutine_traits<::milli::lazy<T>>{
    using promise_type = ::milli::coro::detail::lazy_promise_type<T>;
  };
}
#else
namespace std {
  template <typename T>
  class coroutine_traits<::milli::lazy<T>>{
    using promise_type = ::milli::coro::detail::lazy_promise_type_base<T>;
  };
}
#endif

#endif //MILLI_LIBRARY_SRC_MILLI_CORO_LAZY_H
