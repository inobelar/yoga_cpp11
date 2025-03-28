/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>

namespace facebook {
namespace yoga {

namespace compat {

// Polyfill for std::make_unique<T>() from C++14.
// https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique

namespace detail {

  template<class>
  struct is_unbounded_array { static constexpr bool value = false; };
  template<class T>
  struct is_unbounded_array<T[]> { static constexpr bool value = true; };

  template<class>
  struct is_bounded_array { static constexpr bool value = false; };
  template<class T, std::size_t N>
  struct is_bounded_array<T[N]> { static constexpr bool value = true; };

} // namespace detail

template <class T, class... Args>
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
typename std::enable_if<detail::is_unbounded_array<T>::value, std::unique_ptr<T>>::type
make_unique(std::size_t n)
{
  return std::unique_ptr<T>(new typename std::remove_extent<T>::type [n]());
}

template <class T, class... Args>
typename std::enable_if<detail::is_bounded_array<T>::value>::type
make_unique(Args&&...) = delete;

} // namespace compat

} // namespace yoga
} // namespace facebook
