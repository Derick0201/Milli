/*
$FILE: This file is part of the Milli Library.

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

/*
move_initializer_list.cpp: This file is part of the Milli Library.

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

#define BOOST_TEST_MODULE coro_lazy test

#include <algorithm>
#include <boost/test/included/unit_test.hpp>
#include <memory>
#include <milli/coro/lazy.hpp>
#include <milli/make_container.hpp>
#include <type_traits>
#include <vector>

namespace {
constexpr auto foo_value = 5;
}

milli::lazy<int> foo(){
  co_return foo_value;
}

BOOST_AUTO_TEST_SUITE(coro_lazy_test_suite)


  BOOST_AUTO_TEST_CASE(coroutine_based_lazy) {
    milli::lazy<int> lazy_foo = foo();
    BOOST_ASSERT(not lazy_foo);
    BOOST_ASSERT(not lazy_foo.has_value());

    lazy_foo.initialize();

    BOOST_ASSERT(lazy_foo);
    BOOST_ASSERT(lazy_foo.has_value());
    BOOST_ASSERT(*lazy_foo == foo_value);
  }

  BOOST_AUTO_TEST_CASE(noexcept_correctness) {

  }

  BOOST_AUTO_TEST_CASE(const_correctness){

  }

  BOOST_AUTO_TEST_CASE(constructors){

  }

  BOOST_AUTO_TEST_CASE(value_variants){

  }

  BOOST_AUTO_TEST_CASE(dereference_operator){

  }





BOOST_AUTO_TEST_SUITE_END()