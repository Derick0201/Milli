/*
lazy.cpp: This file is part of the Milli Library.

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

#include <boost/test/included/unit_test.hpp>

#include <milli/lazy.hpp>
#include <milli/raii.hpp>

#include <functional>
#include <stdexcept>
#include <type_traits>


namespace {
  constexpr auto test_value = 42;
}

using assertion_failed = milli::detail::assertion::assertion_failed;

milli::raii exception_guard([] { milli::detail::assertion::rethrow(); });

template<typename T>
constexpr bool is_function_nothrow_move_constructible = std::is_nothrow_move_constructible_v<std::function<T()>>;

struct value_category_checker {
  template<typename T>
  bool is_rvalue(T&& value) { return true; }

  template<typename T>
  bool is_rvalue(T& value) { return false; }
};

BOOST_AUTO_TEST_SUITE(lazy_test_suite)

  BOOST_AUTO_TEST_CASE(construction_with_empty_function_fails) {
    (void) milli::lazy<int>(std::function<int()>{});
    BOOST_CHECK_THROW(milli::detail::assertion::rethrow(), assertion_failed);
  }

  BOOST_AUTO_TEST_CASE(in_place_construction_single_argument_constructor) {
    milli::lazy<int> lazy(std::in_place, test_value);
    BOOST_TEST(lazy.has_value());
    BOOST_TEST(static_cast<bool>(lazy));
    BOOST_TEST(*lazy == test_value);
  }

  BOOST_AUTO_TEST_CASE(in_place_construction_multiargument_constructor) {
    struct point {
      int x;
      int y;

      auto operator==(const point& rhs) const {
        return x == rhs.x && y == rhs.y;
      }
    };

    struct square {
      //testing parameter passing
      square(point& lb, point& rt)
          : left_bottom(lb),
            right_top(rt) {}

      square(int lbx, int lby, int rtx, int rty)
          : left_bottom{lbx, lby},
            right_top{rtx, rty} {}

      point left_bottom;
      point right_top;

      auto operator==(const square& rhs) const {
        return left_bottom == rhs.left_bottom &&
               right_top == rhs.right_top;
      }
    };

    {
      milli::lazy<square> lazy(std::in_place, 1, 2, 3, 4);
      square comparison_square{1, 2, 3, 4};
      BOOST_TEST(lazy.has_value());
      BOOST_TEST(static_cast<bool>(lazy));
      BOOST_TEST((*lazy == comparison_square));
    }
    {
      point a{1, 2}, b{3, 4};
      milli::lazy<square> lazy(std::in_place, a, b);
      square comparison_square{1, 2, 3, 4};
      BOOST_TEST(lazy.has_value());
      BOOST_TEST(static_cast<bool>(lazy));
      BOOST_TEST((*lazy == comparison_square));
    }
  }

  BOOST_AUTO_TEST_CASE(noexcept_correctness) {
    struct throwing {
      throwing(int a, int b);
    };
    constexpr bool in_place_constructor_noexcept = noexcept(milli::lazy<int>{std::in_place, 5});
    constexpr bool in_place_constructor_non_noexcept = not noexcept(milli::lazy<throwing>{std::in_place, 1, 2});

    constexpr bool casual_constructor_nothrow = noexcept(milli::lazy<throwing>{std::function<throwing()>{}});
    constexpr bool casual_constructor_nothrow_if_function_nothrow = is_function_nothrow_move_constructible<throwing>
                                                                    ? casual_constructor_nothrow
                                                                    : not casual_constructor_nothrow;

    constexpr bool has_value_noexcept = noexcept(std::declval<milli::lazy<throwing>>().has_value());
    constexpr bool bool_operator_noexcept = noexcept(std::declval<milli::lazy<throwing>>().operator bool());

    BOOST_TEST(in_place_constructor_noexcept);
    BOOST_TEST(in_place_constructor_non_noexcept);
    BOOST_TEST(casual_constructor_nothrow_if_function_nothrow);

    BOOST_TEST(has_value_noexcept);
    BOOST_TEST(bool_operator_noexcept);

  }

  BOOST_AUTO_TEST_CASE(const_correctness) {
    const milli::lazy const_lazy([] { return test_value; });
    BOOST_TEST(not const_lazy.has_value());
    BOOST_TEST(not const_lazy.operator bool());
  }

  BOOST_AUTO_TEST_CASE(member_types_correctness) {
    struct test {
    };
    using tested_lazy = milli::lazy<test>;
    constexpr bool initializer_proper_type = std::is_same_v<tested_lazy::value_type, test>;
    constexpr bool functor_proper_type = std::is_same_v<tested_lazy::initializer_type, std::function<test()>>;

    BOOST_TEST(initializer_proper_type);
    BOOST_TEST(functor_proper_type);
  }

  template<typename T>
  static constexpr bool is_rvalue(T&& value) { return true; }

  template<typename T>
  static constexpr bool is_rvalue(T& value) { return false; }

  BOOST_AUTO_TEST_CASE(value_variants) {
    milli::lazy<int> lazy(std::in_place, test_value);

    BOOST_ASSERT(lazy.has_value());

    BOOST_TEST(not is_rvalue(lazy.value()));
    BOOST_TEST(is_rvalue(std::move(lazy).value()));

    constexpr bool value_returns_non_const_lreference = std::is_same_v<decltype(lazy.value()), int&>;
    constexpr bool value_returns_non_const_rreference = std::is_same_v<decltype(std::move(lazy).value()), int&&>;

    BOOST_TEST(value_returns_non_const_lreference);
    BOOST_TEST(value_returns_non_const_rreference);

  }

  BOOST_AUTO_TEST_CASE(dereference_operator) {
    milli::lazy<int> lazy(std::in_place, test_value);

    BOOST_ASSERT(lazy.has_value());

    BOOST_TEST(not is_rvalue(*lazy));
    BOOST_TEST(is_rvalue(*std::move(lazy)));

    constexpr bool operator_returns_non_const_lreference = std::is_same_v<decltype(*lazy), int&>;
    constexpr bool operator_returns_non_const_rreference = std::is_same_v<decltype(*std::move(lazy)), int&&>;

    BOOST_TEST(operator_returns_non_const_lreference);
    BOOST_TEST(operator_returns_non_const_rreference);
  }

  BOOST_AUTO_TEST_CASE(arrow_operator) {
    struct test {
      int value_;

      int value() { return value_; }
    };
    milli::lazy lazy([] { return test{test_value}; });

    BOOST_TEST(not lazy.has_value());
    BOOST_TEST(not lazy.operator bool());

    BOOST_TEST(lazy->value() == test_value);

    BOOST_TEST(lazy.has_value());
    BOOST_TEST(lazy.operator bool());

    constexpr bool operator_returns_pointer_to_nonconst = std::is_same_v<decltype(lazy.operator->()), test*>;
    BOOST_TEST(operator_returns_pointer_to_nonconst);
  }

  BOOST_AUTO_TEST_CASE(lazy_initialize_initializes_value) {
    milli::lazy lazy([] { return test_value; });
    BOOST_TEST(not lazy.has_value());
    BOOST_TEST(not lazy.operator bool());

    lazy.initialize();

    BOOST_TEST(lazy.has_value());
    BOOST_TEST(lazy.operator bool());
  }

  BOOST_AUTO_TEST_CASE(lazy_template_can_be_instantiated_with_const_value) {
    milli::lazy<const int> lazy([]() { return test_value; });

    BOOST_TEST(not lazy.has_value());
    BOOST_TEST(not lazy.operator bool());

    lazy.initialize();

    BOOST_TEST(lazy.has_value());
    BOOST_TEST(lazy.operator bool());

    BOOST_TEST(lazy.value() == test_value);

    constexpr bool dereference_returns_const_lreference = std::is_same_v<decltype(*lazy), const int&>;
    constexpr bool dereference_returns_const_rreference = std::is_same_v<decltype(*std::move(lazy)), const int&&>;
    constexpr bool operator_arrow_returns_pointer_to_const = std::is_same_v<decltype(lazy.operator->()), const int*>;
    constexpr bool value_returns_const_lreference = std::is_same_v<decltype(lazy.value()), const int&>;
    constexpr bool value_returns_const_rreference = std::is_same_v<decltype(std::move(lazy).value()), const int&&>;

    BOOST_TEST(dereference_returns_const_lreference);
    BOOST_TEST(dereference_returns_const_rreference);
    BOOST_TEST(operator_arrow_returns_pointer_to_const);
    BOOST_TEST(value_returns_const_lreference);
    BOOST_TEST(value_returns_const_rreference);
  }

#ifdef MILLI_HAS_CORO_SUPPORT

  static milli::lazy<int> foo() {
   co_return test_value;
  }

  BOOST_AUTO_TEST_CASE(coroutine_based_lazy_sanity) {
    milli::lazy lazy_foo = foo();
    BOOST_TEST(not lazy_foo);
    BOOST_TEST(not lazy_foo.has_value());

    lazy_foo.initialize();

    BOOST_TEST(static_cast<bool>(lazy_foo));
    BOOST_TEST(lazy_foo.has_value());
    BOOST_TEST(*lazy_foo == test_value);
  }

#endif


BOOST_AUTO_TEST_SUITE_END()