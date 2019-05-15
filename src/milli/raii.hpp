/*
raii.hpp: This file is part of the Milli Library.

    Copyright (C) Dawid Pilarski, PanicSoftware 2009-2010
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

#ifndef MILLI_RAII_HPP
#define MILLI_RAII_HPP

#include <utility>
#include <type_traits>
#include <optional>

namespace milli {

  template<typename Functor>
  class raii {
  public:

    raii() = delete;

    raii(const raii& rhs) = delete;

    explicit raii(Functor finalizer)
        : functor_(std::in_place, std::move(finalizer)) {}

    raii(raii&& rhs) noexcept(std::is_nothrow_move_constructible_v<Functor>)
    : functor_(std::move(rhs.functor_)) { rhs.functor_.reset(); }

    ~raii() noexcept(std::is_nothrow_invocable_v<Functor>) { if (functor_) functor_.value()(); }

  private:
    std::optional<Functor> functor_;
  };

  template<typename Functor>
  auto make_raii(Functor finalizer) -> raii<Functor> {
    return raii{std::move(finalizer)};
  }

  template<typename Functor>
  raii(Functor&&) -> raii<Functor>;

}


#endif //MILLI_RAII_HPP
