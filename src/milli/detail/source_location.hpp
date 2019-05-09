/*
source_location.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_LIBRARY_SOURCE_LOCATION_HPP
#define MILLI_LIBRARY_SOURCE_LOCATION_HPP

#if __has_include(<experimental/source_location>) || __has_include(<source_location>)

#define MILLI_HAS_SOURCE_LOCATION 1

#if __has_include(<experimental/source_location>)

#include <experimental/source_location>

namespace milli::detail {
  using source_location = std::experimental::source_location;
}

#elif __has_include(<source_location>)
#include <source_location>
namespace milli::detail{
  using source_location = std::source_location;
}

#endif
#endif

#endif //MILLI_LIBRARY_SOURCE_LOCATION_HPP
