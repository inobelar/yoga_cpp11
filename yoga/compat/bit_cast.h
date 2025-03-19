/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstring>
#include <yoga/compat/type_traits.h>

namespace facebook {
namespace yoga {

namespace compat {

// Polyfill for std::bit_cast() from C++20, to allow safe type punning.
// https://en.cppreference.com/w/cpp/numeric/bit_cast
template <class To, class From>
inline To bit_cast(const From& src) noexcept {

  /*
   * NOTE: instead of usage std::enable_if as return type:
   *
   *   typename std::enable_if<
   *     sizeof(To) == sizeof(From) &&
   *     yoga_is_trivially_copyable(From) &&
   *     yoga_is_trivially_copyable(To) &&
   *     yoga_is_trivially_constructible(To),
   *     To
   *   >::type
   *
   * Here is used static_assert's for the same purpose.
   *
   * Because in old compilers (like g++ < 5.0) missing type traits will fallback
   * into 'built-in traits', that cannot be used in function signature, and we
   * can see the next error:
   *
   *   error: use of built-in trait ‘__has_trivial_copy(From)’ in function signature; use library traits instead
   */
  static_assert(sizeof(To) == sizeof(From),          "sizeof(To) != sizeof(From)");
  static_assert(yoga_is_trivially_copyable(From),    "'From' is not trivially copyable");
  static_assert(yoga_is_trivially_copyable(To),      "'To' is not trivially copyable");
  static_assert(yoga_is_trivially_constructible(To), "'To' is not trivially constructible");

  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

} // namespace compat

} // namespace yoga
} // namespace facebook
