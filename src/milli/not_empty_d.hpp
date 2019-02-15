/*
not_empty_d.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_LIBRARY_NOT_EMPTY_D_HPP
#define MILLI_LIBRARY_NOT_EMPTY_D_HPP

#include <milli/not_empty.hpp>
#include <cassert>

namespace milli{

  namespace detail{
    struct debug_error_handler{
      void operator()(bool condition) noexcept {
        assert(condition);
      }
    };
  }

  template <typename T>
  class not_empty_d : public not_empty_base<T, detail::debug_error_handler>{
    using not_empty_base<T, detail::debug_error_handler>::not_empty_base;
  };

#ifdef __cpp_deduction_guides
template<typename T>
not_empty_d(T) -> not_empty_d<T>;
#endif

}


#endif //MILLI_LIBRARY_NOT_EMPTY_D_HPP
