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

/*
  Workaround for missing `type traits` in g++ < 5.0

  References:
  - https://stackoverflow.com/questions/25123458/is-trivially-copyable-is-not-a-member-of-std
  - https://github.com/harfbuzz/harfbuzz/blob/0349359ce67f9b1554d9fb46c057bbb492997132/src/hb-meta.hh#L202-L214
*/

#if defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define yoga_is_trivially_copyable(T)           __has_trivial_copy(T)
#define yoga_is_trivially_copy_assignable(T)    __has_trivial_assign(T)
#define yoga_is_trivially_constructible(T)      __has_trivial_constructor(T)
#define yoga_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define yoga_is_trivially_destructible(T)       __has_trivial_destructor(T)
#else
#define yoga_is_trivially_copyable(T)           std::is_trivially_copyable<T>::value
#define yoga_is_trivially_copy_assignable(T)    std::is_trivially_copy_assignable<T>::value
#define yoga_is_trivially_constructible(T)      std::is_trivially_constructible<T>::value
#define yoga_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define yoga_is_trivially_destructible(T)       std::is_trivially_destructible<T>::value
#endif

} // namespace compat

} // namespace yoga
} // namespace facebook
