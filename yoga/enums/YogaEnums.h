/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <iterator>
#include <type_traits>

#include <yoga/compat/bit_width.h>

namespace facebook {
namespace yoga {

/**
 * Concept for any enum/enum class
 */
template <typename EnumT>
constexpr bool IsEnumeration() { return std::is_enum<EnumT>::value; }

/**
 * Count of ordinals in a Yoga enum which is sequential
 */
template </*Enumeration*/ typename EnumT>
constexpr int32_t ordinalCount();

/**
 * Concept for a yoga enum which is sequential
 */
template <typename EnumT>
constexpr bool HasOrdinality() { return ordinalCount<EnumT>() > 0; }


/**
 * Count of bits needed to represent every ordinal
 */
template <typename EnumT>
constexpr int32_t bitCount() {
  static_assert(HasOrdinality<EnumT>() == true, "EnumT not has ordinality");
  return compat::bit_width(
      static_cast<typename std::underlying_type<EnumT>::type>(ordinalCount<EnumT>() - 1));
}

/**
 * Polyfill of C++ 23 to_underlying()
 * https://en.cppreference.com/w/cpp/utility/to_underlying
 */
template <typename EnumT>
constexpr
typename std::underlying_type<EnumT>::type
to_underlying(EnumT e) noexcept {
  static_assert(IsEnumeration<EnumT>() == true, "EnumT must be an enum");
  return static_cast<typename std::underlying_type<EnumT>::type>(e);
}

namespace detail {

  template <typename EnumT>
  struct Iterator {
    EnumT e{};

    EnumT operator*() const {
      return e;
    }

    Iterator& operator++() {
      e = static_cast<EnumT>(to_underlying(e) + 1);
      return *this;
    }

    bool operator==(const Iterator& other) const { return this->e == other.e; }
    bool operator!=(const Iterator& other) const { return this->e != other.e; }
  };

  template <typename EnumT>
  struct Range {
    using EnumIterator = Iterator<EnumT>;

    EnumIterator begin() const {
      return EnumIterator{};
    }
    EnumIterator end() const {
      return EnumIterator{static_cast<EnumT>(ordinalCount<EnumT>())};
    }
  };

} // namespace detail

/**
 * Convenience function to iterate through every value in a Yoga enum as part of
 * a range-based for loop.
 */
template <typename EnumT>
detail::Range<EnumT> ordinals()
{
  static_assert(HasOrdinality<EnumT>() == true, "EnumT not has ordinality");
  return detail::Range<EnumT>{};
}

} // namespace yoga
} // namespace facebook
