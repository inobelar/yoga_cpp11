/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstring>
#include <type_traits>

namespace facebook {
namespace yoga {

namespace compat {

// Polyfill for std::bit_cast() from C++20, to allow safe type punning.
// https://en.cppreference.com/w/cpp/numeric/bit_cast
template <class To, class From>
typename std::enable_if<
    sizeof(To) == sizeof(From) &&
    std::is_trivially_copyable<From>::value &&
    std::is_trivially_copyable<To>::value &&
    std::is_trivially_constructible<To>::value,
    To
>::type
inline bit_cast(const From& src) noexcept {
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

} // namespace compat

} // namespace yoga
} // namespace facebook
