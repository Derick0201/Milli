/*
strong_assert.cpp: This file is part of the Milli Library.

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

#define BOOST_TEST_MODULE strong_assert test

#include <boost/test/included/unit_test.hpp>
#include <milli/make_container.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include <type_traits>
#include <milli/detail/assert.hpp>

milli::raii exception_guard([] { milli::detail::assertion::rethrow(); });

BOOST_AUTO_TEST_SUITE(strong_assert_test_suite)

  BOOST_AUTO_TEST_CASE(no_element_initialization) {
    milli::make_container<std::vector<int>>();
  }

BOOST_AUTO_TEST_SUITE_END()
