/*
optional.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_OPTIONAL_HPP
#define MILLI_OPTIONAL_HPP

#ifdef __cpp_lib_optional

#include <optional>
namespace milli::detail{
  template <typename T>
  using optional = std::optional;
}

#else

#include <utility>
#include "strong_assert.hpp"

namespace milli {

  namespace detail {

    template<typename T>
    struct optional {

      optional() noexcept : has_value_(false) {}

      optional(T&& value) noexcept(std::is_nothrow_constructible<T, decltype(std::forward<T>(value))>::value) {
        new(&data_) T(std::forward<T>(value));
        has_value_ = true;
      }

      optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible<T>::value) {
        new(&data_) T(std::move(rhs.get()));
        rhs.reset();
        has_value_ = true;
      }

      void reset() noexcept(noexcept(std::declval<T>().~T())) {
        get().~T();
        has_value_ = false;
      }

      auto get() noexcept -> T& {
        strong_assert(has_value_);
        return *reinterpret_cast<T*>(&data_);
      }

      auto get() const noexcept -> const T& {
        strong_assert(has_value_);
        return const_cast<optional*>(this)->get();
      }

      auto empty() noexcept -> bool {
        return not(*this);
      }

      explicit operator bool() const noexcept {
        return has_value_;
      }

      auto operator->() noexcept -> T* {
        strong_assert(has_value_);
        return &get();
      }

      auto operator->() const noexcept -> T const* {
        return &get();
      }

      auto operator*() noexcept -> T& {
        strong_assert(has_value_);
        return get();
      }

      auto operator*() const noexcept -> T const& {
        return get();
      }

    private:
      using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
      Storage data_;
      bool has_value_;
    };

  }
}

#endif // __cpp_lib_optional
#endif //MILLI_OPTIONAL_HPP
