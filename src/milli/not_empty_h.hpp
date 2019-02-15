/*
not_empty_h.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_LIBRARY_NOT_EMPTY_H_HPP
#define MILLI_LIBRARY_NOT_EMPTY_H_HPP

#include <milli/not_empty.hpp>
#include <exception>
#include "strong_assert.hpp"

namespace milli{
  namespace detail{
    struct hard_error_handler{
      void operator()(bool condition) noexcept {
        strong_assert(condition, "milli::detail::not_empty_h. Value in not_empty was empty", __PRETTY_FUNCTION__);
      }
    };
  }

  template <typename T>
  class not_empty_h : public not_empty_base<T, detail::hard_error_handler>{
    using not_empty_base<T, detail::hard_error_handler>::not_empty_base;
  };

#ifdef __cpp_deduction_guides
template<typename T>
not_empty_h(T) -> not_empty_h<T>;
#endif
}

#endif //MILLI_LIBRARY_NOT_EMPTY_H_HPP
