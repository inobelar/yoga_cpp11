/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// @generated by enums.py
// clang-format off
#pragma once

#include <cstdint>
#include <yoga/YGEnums.h>
#include <yoga/enums/YogaEnums.h>

namespace facebook {
namespace yoga {

enum class MeasureMode : uint8_t {
  Undefined = YGMeasureModeUndefined,
  Exactly = YGMeasureModeExactly,
  AtMost = YGMeasureModeAtMost,
};

template <>
constexpr int32_t ordinalCount<MeasureMode>() {
  return 3;
}

constexpr MeasureMode scopedEnum(YGMeasureMode unscoped) {
  return static_cast<MeasureMode>(unscoped);
}

constexpr YGMeasureMode unscopedEnum(MeasureMode scoped) {
  return static_cast<YGMeasureMode>(scoped);
}

inline const char* toString(MeasureMode e) {
  return YGMeasureModeToString(unscopedEnum(e));
}

} // namespace yoga
} // namespace facebook
