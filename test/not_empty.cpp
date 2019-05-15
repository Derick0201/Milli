/*
not_empty.cpp: This file is part of the Milli Library.

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

#include <milli/not_empty_e.hpp>
#include <milli/not_empty_d.hpp>
#include <milli/not_empty_h.hpp>
#include <milli/not_empty_i.hpp> // syntax check

#define BOOST_TEST_MODULE not_empty test
#include <boost/test/included/unit_test.hpp>

using namespace milli;

milli::raii exception_guard([] { milli::detail::assertion::rethrow(); });

BOOST_AUTO_TEST_SUITE(not_empty_test_suite)

  BOOST_AUTO_TEST_CASE(no_default_constructor) {
    constexpr bool is_default_constructible = std::is_default_constructible<not_empty_e<int*>>::value;
    BOOST_TEST(is_default_constructible == false);
  }

  BOOST_AUTO_TEST_CASE(no_nullptr_t_constructor) {
    constexpr bool is_nullptr_constructible = std::is_constructible<not_empty_e<int*>, std::nullptr_t>::value;
    constexpr bool is_nullptr_constructible_ref = std::is_constructible<not_empty_e<int*>, std::nullptr_t&>::value;
    constexpr bool is_nullptr_constructible_rref = std::is_constructible<not_empty_e<int*>, std::nullptr_t&&>::value;
    constexpr bool is_nullptr_constructible_cref = std::is_constructible<not_empty_e<int*>, const std::nullptr_t&>::value;
    constexpr bool is_nullptr_constructible_crref = std::is_constructible<not_empty_e<int*>, const std::nullptr_t&&>::value;

    BOOST_TEST(is_nullptr_constructible == false);
    BOOST_TEST(is_nullptr_constructible_ref == false);
    BOOST_TEST(is_nullptr_constructible_rref == false);
    BOOST_TEST(is_nullptr_constructible_cref == false);
    BOOST_TEST(is_nullptr_constructible_crref == false);
  }

#ifdef __cpp_lib_optional

  BOOST_AUTO_TEST_CASE(no_nullopt_t_constructor) {
    constexpr bool is_nullopt_constructible = std::is_constructible<not_empty_e<int*>, std::nullopt_t>::value;
    constexpr bool is_nullopt_constructible_ref = std::is_constructible<not_empty_e<int*>, std::nullopt_t&>::value;
    constexpr bool is_nullopt_constructible_rref = std::is_constructible<not_empty_e<int*>, std::nullopt_t&&>::value;
    constexpr bool is_nullopt_constructible_cref = std::is_constructible<not_empty_e<int*>, const std::nullopt_t&>::value;
    constexpr bool is_nullopt_constructible_crref = std::is_constructible<not_empty_e<int*>, const std::nullopt_t&&>::value;

    BOOST_TEST(is_nullopt_constructible == false);
    BOOST_TEST(is_nullopt_constructible_ref == false);
    BOOST_TEST(is_nullopt_constructible_rref == false);
    BOOST_TEST(is_nullopt_constructible_cref == false);
    BOOST_TEST(is_nullopt_constructible_crref == false);
  }

#endif

  BOOST_AUTO_TEST_CASE(null_pointer_initialization_fails) {
    int* ptr = nullptr;
    BOOST_CHECK_THROW(not_empty_e<int*> test(ptr), milli::value_not_empty);
  }

  BOOST_AUTO_TEST_CASE(constructor_test) {
    int test_value;
    not_empty_e<int*> intptr(&test_value);
    *intptr = 1;

    BOOST_TEST(test_value == 1);
  }


  BOOST_AUTO_TEST_CASE(smart_ptr_empty_test) {
    std::unique_ptr<int> my_test;
    BOOST_CHECK_THROW(not_empty_e<std::unique_ptr<int>> not_null_ptr(std::move(my_test)), milli::value_not_empty);
  }

  BOOST_AUTO_TEST_CASE(smart_ptr_test) {
    std::unique_ptr<int> my_test(new int);
    {
      not_empty_e<std::unique_ptr<int>> not_null_ptr(std::move(my_test));
    }

    BOOST_TEST(not my_test);
  }

#ifdef __cpp_lib_optional

  BOOST_AUTO_TEST_CASE(optional_empty_type_test) {
    std::optional<int> empty_optional;
    BOOST_CHECK_THROW(not_empty_e<std::optional<int>> not_null_optional(empty_optional), milli::value_not_empty);
  }

  BOOST_AUTO_TEST_CASE(optional_type_test) {
    std::optional<int> empty_optional(5);
    not_empty_e<std::optional<int>> not_null_optional(empty_optional);

    BOOST_TEST(*not_null_optional == 5);
  }

#endif

#ifdef __cpp_deduction_guides

  BOOST_AUTO_TEST_CASE(deduction_guide) {
    not_empty_e value(new int);

    constexpr bool deduction_type_check = std::is_same<decltype(value), not_empty_e<int*>>::value;
    BOOST_TEST(deduction_type_check);
    delete value.get();
  }

#endif

  BOOST_AUTO_TEST_CASE(simple_assignment_operator){
    int first = 1;
    int second = 2;
    not_empty_h<int*> test(&first);
    test = &second;
    BOOST_TEST(*test == second);

    constexpr bool noexcept_raw_pointer = noexcept(test = &second);
    BOOST_TEST(noexcept_raw_pointer);

    test = not_empty_e<int*>(&first);
    BOOST_TEST(*test == first);

    constexpr bool noexcept_not_empty_pointer = noexcept(test = not_empty_h<int*>(&first));
    BOOST_TEST(noexcept_not_empty_pointer);
  }

  BOOST_AUTO_TEST_CASE(complex_assignment_operator) {

    const auto shared_val_1 = std::make_shared<int>(1);
    const auto shared_val_2 = std::make_shared<int>(2);
    not_empty_e<std::shared_ptr<int>> shared_test = shared_val_1;
    static_assert(std::is_convertible<decltype(shared_test), std::shared_ptr<int>>::value, "test");
    shared_test = shared_val_2;
    BOOST_TEST(*shared_test == *shared_val_2);

    struct A : std::shared_ptr<int> {
      A& operator=(const A& rhs) noexcept(false) { return *this; }
    };

    BOOST_CHECK_THROW(not_empty_e<A> throwable_test(A{}), milli::value_not_empty);
  }

  BOOST_AUTO_TEST_CASE(no_nullptr_assignment) {
    constexpr bool is_nullptr_assignable = std::is_assignable<not_empty_e<int*>, std::nullptr_t>::value;
    constexpr bool is_nullptr_assignable_ref = std::is_assignable<not_empty_e<int*>, std::nullptr_t&>::value;
    constexpr bool is_nullptr_assignable_rref = std::is_assignable<not_empty_e<int*>, std::nullptr_t&&>::value;
    constexpr bool is_nullptr_assignable_cref = std::is_assignable<not_empty_e<int*>, const std::nullptr_t&>::value;
    constexpr bool is_nullptr_assignable_crref = std::is_assignable<not_empty_e<int*>, const std::nullptr_t&&>::value;

    BOOST_TEST(is_nullptr_assignable == false);
    BOOST_TEST(is_nullptr_assignable_ref == false);
    BOOST_TEST(is_nullptr_assignable_rref == false);
    BOOST_TEST(is_nullptr_assignable_cref == false);
    BOOST_TEST(is_nullptr_assignable_crref == false);
  }

#ifdef __cpp_lib_optional

  BOOST_AUTO_TEST_CASE(no_nullopt_assignment) {
    constexpr bool is_nullopt_assignable = std::is_assignable<not_empty_e<int*>, std::nullopt_t>::value;
    constexpr bool is_nullopt_assignable_ref = std::is_assignable<not_empty_e<int*>, std::nullopt_t&>::value;
    constexpr bool is_nullopt_assignable_rref = std::is_assignable<not_empty_e<int*>, std::nullopt_t&&>::value;
    constexpr bool is_nullopt_assignable_cref = std::is_assignable<not_empty_e<int*>, const std::nullopt_t&>::value;
    constexpr bool is_nullopt_assignable_crref = std::is_assignable<not_empty_e<int*>, const std::nullopt_t&&>::value;

    BOOST_TEST(is_nullopt_assignable == false);
    BOOST_TEST(is_nullopt_assignable_ref == false);
    BOOST_TEST(is_nullopt_assignable_rref == false);
    BOOST_TEST(is_nullopt_assignable_cref == false);
    BOOST_TEST(is_nullopt_assignable_crref == false);
  }

#endif

  BOOST_AUTO_TEST_CASE(constructor_accepts_pointer_of_child_type) {
    struct A {
    };
    struct B : A {
    };

    A test_parent;
    B test_child;

    not_empty_e<A*> parent_ptr(&test_parent);
    not_empty_e<B*> child_ptr(&test_child);

    not_empty_e<A*> result_ptr(child_ptr);

    BOOST_TEST(parent_ptr != child_ptr);
    BOOST_TEST(result_ptr == child_ptr);

    not_empty_e<A*> direct_initialized(&test_child);
    BOOST_TEST(direct_initialized == result_ptr);
    BOOST_TEST(direct_initialized == child_ptr);
  }

  BOOST_AUTO_TEST_CASE(cast_operator) {
    auto ptr = std::make_shared<int>(1);

    not_empty_e<std::shared_ptr<int>> not_empty(ptr);
    std::shared_ptr<int> init_test = not_empty;
  }

  BOOST_AUTO_TEST_CASE(get_member_function) {
    int value;
    not_empty_h<int*> test(&value);

    BOOST_TEST(&value == test.get());

    constexpr bool non_const_pointer = std::is_same<decltype(test.get()), int*&>::value;
    BOOST_TEST(non_const_pointer);

    const auto& const_test = test;
    constexpr bool const_pointer = std::is_same<decltype(const_test.get()), int* const&>::value;
    BOOST_TEST(const_pointer);

    constexpr bool noexcept_const_get = noexcept(const_test.get());
    constexpr bool noexcept_get = noexcept(test.get());

    BOOST_TEST(noexcept_const_get);
    BOOST_TEST(noexcept_get);
  }

  BOOST_AUTO_TEST_CASE(dereference_operator) {
    int value=1;
    not_empty_h<int*> test(&value);

    BOOST_TEST(*test == value);
    BOOST_TEST(*test == 1);

    const not_empty_h<int*>& const_test = test;

    constexpr bool noexcept_dereference = noexcept(*test);
    constexpr bool noexcept_const_dereference = noexcept(*const_test);

    BOOST_TEST(noexcept_dereference == true);
    BOOST_TEST(noexcept_const_dereference == true);
  }

  BOOST_AUTO_TEST_CASE(casting_operator){
    int test=1;
    not_empty_e<int*> test_ne(&test);

    int& test_ref = *test_ne;

    BOOST_TEST(test_ref == test);
    BOOST_TEST(&test_ref == &test);
    BOOST_TEST(*test_ne == test);

    const not_empty_e<int*>& const_test_ne = test_ne;
    constexpr bool noexcept_cast_op = noexcept(static_cast<int*&>(test_ne));
    constexpr bool noexcept_const_cast_op = noexcept(static_cast<int* const&>(const_test_ne));

    BOOST_TEST(noexcept_cast_op == true);
    BOOST_TEST(noexcept_const_cast_op == true);
  }

  BOOST_AUTO_TEST_CASE(member_types) {
    using tested_type = not_empty_e<int*>;
    constexpr bool simple_pointer_element_type = std::is_same<tested_type::element_type, int>::value;
    constexpr bool simple_pointer_stored_type = std::is_same<not_empty_e<int*>::stored_type, int*>::value;
    BOOST_TEST(simple_pointer_element_type);
    BOOST_TEST(simple_pointer_stored_type);
  }

  BOOST_AUTO_TEST_CASE(arrow_dereference) {

    struct Foo{
      int function(){return 1;}
    };

    not_empty_e<std::shared_ptr<Foo>> test(std::shared_ptr<Foo>(new Foo));
    BOOST_TEST(test->function() == 1);
  }

BOOST_AUTO_TEST_SUITE_END()