/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <iterator>

namespace facebook {
namespace yoga {

namespace compat {

namespace detail {

  // random-access iterator implementation (allows quick range size detection)
  template <class RandomIt1, class RandomIt2, class BinaryPred>
  // constexpr //< since C++20
  bool equal(
    RandomIt1 first1, RandomIt1 last1,
    RandomIt2 first2, RandomIt2 last2, BinaryPred p,
    std::random_access_iterator_tag, std::random_access_iterator_tag
  )
  {
    if (last1 - first1 != last2 - first2)
      return false;

    for (; first1 != last1; ++first1, ++first2)
      if (!p(*first1, *first2))
        return false;

    return true;
  }

  // input iterator implementation (needs to manually compare with “last2”)
  template <class InputIt1, class InputIt2, class BinaryPred>
  // constexpr //< since C++20
  bool equal(
    InputIt1 first1, InputIt1 last1,
    InputIt2 first2, InputIt2 last2, BinaryPred p,
    std::input_iterator_tag, std::input_iterator_tag
  )
  {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
      if (!p(*first1, *first2))
        return false;

    return first1 == last1 && first2 == last2;
  }

} // namespace detail

// Polyfill for std::equal() from C++17.
// https://en.cppreference.com/w/cpp/algorithm/equal
template <class InputIt1, class InputIt2, class BinaryPred>
constexpr //< since C++20
bool equal(InputIt1 first1, InputIt1 last1,
           InputIt2 first2, InputIt2 last2, BinaryPred p)
{
  return detail::equal(
    first1, last1, first2, last2, p,
    typename std::iterator_traits<InputIt1>::iterator_category(),
    typename std::iterator_traits<InputIt2>::iterator_category()
  );
}

} // namespace compat

} // namespace yoga
} // namespace facebook
