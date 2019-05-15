/*
raii.cpp: This file is part of the Milli Library.

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

#define BOOST_TEST_MODULE raii test

#include <boost/test/included/unit_test.hpp>
#include <milli/raii.hpp>
#include <milli/strong_assert.hpp>
#include <milli/detail/assert.hpp>
#include <functional>

using namespace milli;

milli::raii exception_guard([] { milli::detail::assertion::rethrow(); });


//todo improve raii code
BOOST_AUTO_TEST_SUITE(raii_test_suite)

  BOOST_AUTO_TEST_CASE(simple_raii){
    int test = 0;
    make_raii([&test](){ test = 2; });

    BOOST_TEST(test == 2, "raii function gets called successfully for temporary variable");
  }

  BOOST_AUTO_TEST_CASE(simple_raii_lifetime){
    int test = 0;

    {
      auto raii = make_raii([&test](){ test *= 2; });
      test = 2;
    }

    BOOST_TEST(test == 4, "raii function gets called successfully for scoped variable");
  }

  BOOST_AUTO_TEST_CASE(empty_std_function_raii){
    BOOST_CHECK_THROW(make_raii(std::function<void()>()), std::bad_function_call);
  }

  void nop(){}

  BOOST_AUTO_TEST_CASE(stateless_raii){
    make_raii(nop);
  }

  BOOST_AUTO_TEST_CASE(noexceptness_test){
    auto noexcept_lambda = []() noexcept {};
    auto except_lambda = []() {};
    struct functor{
      functor() = default;
      functor(const functor&) noexcept = default;
      functor(functor&& rhs) noexcept = default;
    };

    static_assert(std::is_nothrow_move_constructible_v<decltype(noexcept_lambda)>);
    static_assert(std::is_nothrow_move_constructible_v<decltype(except_lambda)>);
    static_assert(std::is_nothrow_move_constructible_v<std::optional<decltype(noexcept_lambda)>>);
    static_assert(std::is_nothrow_move_constructible_v<std::optional<decltype(except_lambda)>>);
    static_assert(std::is_nothrow_move_constructible_v<raii<decltype(noexcept_lambda)>>);
    static_assert(std::is_nothrow_move_constructible_v<raii<decltype(except_lambda)>>);

    //constexpr bool is_noexcept_constructible_from_lambda = std::is_nothrow_constructible<raii<decltype(noexcept_lambda)>, decltype(noexcept_lambda)>::value;
    //constexpr bool is_except_false_constructible_from_lambda = std::is_nothrow_constructible<raii<decltype(except_lambda)>, decltype(except_lambda)>::value;
    constexpr bool is_noexcept_move_constructible_from_lambda = std::is_nothrow_move_constructible_v<raii<decltype(except_lambda)>>;
    constexpr bool is_noexcept_move_constructible = std::is_nothrow_move_constructible_v<raii<functor>>;

    BOOST_TEST(is_noexcept_move_constructible);
  }

#ifdef __cpp_deduction_guides
  BOOST_AUTO_TEST_CASE(deduction_guides){
    raii test([](){});
  }
#endif

BOOST_AUTO_TEST_SUITE_END()