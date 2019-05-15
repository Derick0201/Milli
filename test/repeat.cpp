/*
repeat.cpp: This file is part of the Milli Library.

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

#define BOOST_TEST_MODULE repeat test

#include <boost/test/included/unit_test.hpp>
#include <milli/repeat.hpp>
#include <limits>
#include <milli/detail/assert.hpp>

constexpr unsigned int repeat_times = 42;

milli::raii exception_guard([] { milli::detail::assertion::rethrow(); });

BOOST_AUTO_TEST_SUITE(repeat_test_suite)

  BOOST_AUTO_TEST_CASE(parameterless_repeat_lambda) {
    int tested_value = 0;
    milli::repeat(repeat_times, [&tested_value] { tested_value++; });

    BOOST_TEST(tested_value == repeat_times);

  }

  BOOST_AUTO_TEST_CASE(parametrized_repeat_lambda) {
    int tested_value = 0;
    milli::repeat(repeat_times, [&tested_value](int parameter_value) {
      BOOST_TEST(parameter_value == tested_value);
      tested_value++;
    });

    BOOST_TEST(tested_value == repeat_times);
  }

  BOOST_AUTO_TEST_CASE(zero_repeat_times){
    constexpr int zero_repeat_times=0;
    int value=0;
    milli::repeat(zero_repeat_times, [&value]{value++;});

    BOOST_TEST(value == 0);
  }

  struct tested_type {
    static int counter;

    tested_type() {
      counter++;
    }

    tested_type(const tested_type& rhs) : tested_type() {}

    void operator()() {

    }
  };

  int tested_type::counter = 0;

  BOOST_AUTO_TEST_CASE(repeat_copies_functor_once) {
    milli::repeat(repeat_times, tested_type());

    BOOST_TEST(tested_type::counter == 1);
  }

  struct functor{ void operator()(){} };
  struct non_throwing_functor{ void operator()() noexcept {} };
  struct parametrized_functor{ void operator()(int){} };
  struct non_throwing_parametrized_functor{ void operator()(int) noexcept{} };

  BOOST_AUTO_TEST_CASE(noexceptness_test){
    BOOST_TEST(!noexcept(milli::repeat(1, functor())));
    BOOST_TEST(noexcept(milli::repeat(1, non_throwing_functor())));

    BOOST_TEST(!noexcept(milli::repeat(1, parametrized_functor())));
    BOOST_TEST(noexcept(milli::repeat(1, non_throwing_parametrized_functor())));
  }

BOOST_AUTO_TEST_SUITE_END()
