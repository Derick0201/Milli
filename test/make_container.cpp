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

#define BOOST_TEST_MODULE make_container test

#include <boost/test/included/unit_test.hpp>
#include <milli/make_container.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include <type_traits>

//todo measurements are needed to compare with initializer_list efficiency
BOOST_AUTO_TEST_SUITE(make_container_test_suite)

BOOST_AUTO_TEST_CASE(no_element_initialization) {
  milli::make_container<std::vector<int>>();
}

BOOST_AUTO_TEST_CASE(couple_element_initialization) {
  milli::make_container<std::vector<int>>(1);
  milli::make_container<std::vector<int>>(1, 2);
  milli::make_container<std::vector<int>>(1, 2, 3);
  milli::make_container<std::vector<int>>(1, 2, 3, 4);
}

BOOST_AUTO_TEST_CASE(indirect_element_initialization) {
  milli::make_container<std::vector<std::unique_ptr<int>>>(new int(2), new int(3), new int(4));
}

BOOST_AUTO_TEST_CASE(container_from_references) {
  const int& a = 1;
  const int& b = 2;
  const int& c = 3;
  const int& d = 4;
  milli::make_container<std::vector<int>>(a, b, d, c);
}

BOOST_AUTO_TEST_CASE(container_from_initializer_lists) {
  std::initializer_list<int> valueOne = {1, 2, 3};
  std::initializer_list<int> valueTwo = {4, 5, 6};
  auto container = milli::make_container<std::vector<std::initializer_list<int>>>(valueOne, valueTwo);

  bool proper_type_deduced = std::is_same<std::vector<std::initializer_list<int>>, decltype(container)>::value;
  BOOST_TEST(proper_type_deduced);

  BOOST_TEST(container.size() == 2);
  BOOST_TEST(std::equal(container[0].begin(), container[0].end(), valueOne.begin()));
  BOOST_TEST(std::equal(container[1].begin(), container[1].end(), valueTwo.begin()));
}

BOOST_AUTO_TEST_SUITE_END()