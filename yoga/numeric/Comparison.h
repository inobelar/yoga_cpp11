/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cmath>

#include <yoga/Yoga.h>

namespace facebook {
namespace yoga {

template <typename T>
constexpr bool isUndefined(T value) {
  static_assert(std::is_floating_point<T>::value == true, "T must be floating-point");
  return value != value;
}

template <typename T>
constexpr bool isDefined(T value) {
  static_assert(std::is_floating_point<T>::value == true, "T must be floating-point");
  return !isUndefined(value);
}

/**
 * Constexpr version of `std::isinf` before C++ 23
 */
template <typename T>
constexpr bool isinf(T value) {
  return value == +std::numeric_limits<T>::infinity() ||
      value == -std::numeric_limits<T>::infinity();
}

template <typename T>
constexpr auto maxOrDefined(
    T a,
    T b
) -> T
{
  static_assert(std::is_floating_point<T>::value == true, "T must be floating-point");
  return
    (yoga::isDefined(a) && yoga::isDefined(b)) ?
      std::max(a, b)
    :
      yoga::isUndefined(a) ? b : a;
}

template <typename T>
constexpr auto minOrDefined(
    T a,
    T b
) -> T
{
  static_assert(std::is_floating_point<T>::value == true, "T must be floating-point");
  return
    (yoga::isDefined(a) && yoga::isDefined(b)) ?
      std::min(a, b)
    :
      yoga::isUndefined(a) ? b : a;
}

// Custom equality functions using a hardcoded epsilon of 0.0001f, or returning
// true if both floats are NaN.
inline bool inexactEquals(float a, float b) {
  if (yoga::isDefined(a) && yoga::isDefined(b)) {
    return std::abs(a - b) < 0.0001f;
  }
  return yoga::isUndefined(a) && yoga::isUndefined(b);
}

inline bool inexactEquals(double a, double b) {
  if (yoga::isDefined(a) && yoga::isDefined(b)) {
    return std::abs(a - b) < 0.0001;
  }
  return yoga::isUndefined(a) && yoga::isUndefined(b);
}

template <std::size_t Size, typename ElementT>
bool inexactEquals(
    const std::array<ElementT, Size>& val1,
    const std::array<ElementT, Size>& val2) {
  bool areEqual = true;
  for (std::size_t i = 0; i < Size && areEqual; ++i) {
    areEqual = inexactEquals(val1[i], val2[i]);
  }
  return areEqual;
}

} // namespace yoga
} // namespace facebook
