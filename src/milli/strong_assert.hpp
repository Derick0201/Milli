/*
strong_assert.hpp: This file is part of the Milli Library.

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

#ifndef MILLI_STRONG_ASSERT_HPP
#define MILLI_STRONG_ASSERT_HPP

#include <iostream>
#include <exception>

namespace milli{

static void strong_assert(bool condition, const char* message = nullptr, const char* error_location = nullptr) noexcept {
  if(not condition){
    if(error_location)
      std::cerr << error_location << " :=> ";
    if(message)
      std::cerr << message;

    std::cerr << std::endl;

    std::terminate();
  }
}

}

#endif //MILLI_STRONG_ASSERT_HPP
