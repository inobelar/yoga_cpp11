/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

namespace facebook {
namespace yoga {

namespace compat {

namespace details {

template <typename T>
constexpr typename std::make_unsigned<T>::type to_unsigned(T value) {
  return static_cast<typename std::make_unsigned<T>::type>(value);
}

// Recursive constexpr bit_width() for C++11
template <typename T>
constexpr int bit_width_recursive(T value) {
  return
    (value == 0) ?
      0
    :
      1 + bit_width_recursive(value >> 1);
}

} // namespace details

// Polyfill for std::bit_width() from C++20.
// https://en.cppreference.com/w/cpp/numeric/bit_width
template <typename T>
constexpr int bit_width(T value) {
  /*
    // Non-recursive, but not compatible with C++11-constexpr'ness algorithm:
    U x = to_unsigned(value);
    int width = 0;
    while (x != 0) {
        x >>= 1;
        ++width;
    }
    return width;
  */

  // Convert to unsigned to avoid complications with right shifting signed types
  using U = typename std::make_unsigned<T>::type;
  return details::bit_width_recursive<U>( details::to_unsigned(value) );
}

} // namespace compat

} // namespace yoga
} // namespace facebook
