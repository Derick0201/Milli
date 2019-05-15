/*
has_coro_support.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_LIBRARY_HAS_CORO_SUPPORT_HPP
#define MILLI_LIBRARY_HAS_CORO_SUPPORT_HPP

#if __has_include(<experimental/coroutine>) or __has_include(<coroutine>)
#define MILLI_HAS_CORO_SUPPORT 1
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

#endif //MILLI_LIBRARY_HAS_CORO_SUPPORT_HPP
