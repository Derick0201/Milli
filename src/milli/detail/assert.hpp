/*
assert.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_LIBRARY_ASSERT_HPP
#define MILLI_LIBRARY_ASSERT_HPP

#ifndef MILLI_TEST
#include <cassert>
#define milli_assert assert
#else

#include <exception>
#include <milli/strong_assert.hpp>

namespace milli::detail::assertion {
  struct assertion_failed : std::logic_error {
    using logic_error::logic_error;
  };

  std::exception_ptr thrown_exception;

  void fail(const char* location) {
    if(thrown_exception){
      strong_assert("unhandled previous exception");
    }

    try {
      throw assertion_failed{location};
    } catch (const assertion_failed&) {
      thrown_exception = std::current_exception();
    }
  }

  void rethrow() {
    if (thrown_exception) {
      auto exception_cpy = thrown_exception;
      thrown_exception = nullptr;

      std::rethrow_exception(exception_cpy);
    }
  }

}


#define MILLI_TO_STRING(value) #value
#define MILLI_TO_STRING_(value)  MILLI_TO_STRING(value)
#define MILLI_LINE MILLI_TO_STRING_(__LINE__)
#define milli_assert(condition) do{ \
                                    if(not (condition)) \
                                      milli::detail::assertion::fail( __FILE__ ":" MILLI_LINE ); \
                                }while(0)

#endif

#endif //MILLI_LIBRARY_ASSERT_HPP
