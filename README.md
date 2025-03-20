# Yoga (C++11 Backport)

Yoga is an embeddable and performant flexbox layout engine with bindings for multiple languages.

This repository provides a backport of [Yoga](https://github.com/facebook/yoga)
from `C++20` to `C++11`, enabling broader compatibility with older compilers and
outdated environments.

By maintaining the core functionality of Yoga while relying on `C++11` features,
this project aims to make the library accessible to a wider range of developers
and systems, especially those that do not support the latest compilers and C++
standards.

In our imperfect world, the number of such systems is greater than it seems, and
such a useful library as Yoga should be distributed as widely as possible.

> [!TIP]
> Current state: based on [6455a84](https://github.com/facebook/yoga/commit/6455a848a76f433bdb48b2640b7f4644774c76fd) commit (Mar 11, 2025)

> [!NOTE]
> Tested on `GCC 4.9.2` (Released: October 30, 2014)

> [!IMPORTANT]
> The main implementation targets `C++11`, but to build the unit tests
> (which rely on [GoogleTest](https://github.com/google/googletest)) needed
> `C++17` support.

<details>
  <summary>Notable changes description (compared to mainstream Yoga (C++20))</summary>

  - `yogacore` (Yoga C++ library) now forced to build with C++11 standard. See [/yoga/CMakeLists.txt](./yoga/CMakeLists.txt)

  - Nested namespaces described explicitly:
    ```c++
    // C++20
    namespace facebook::yoga {
    ...
    } // namespace facebook::yoga

    // C++11
    namespace facebook {
    namespace yoga {
    ...
    } // namespace yoga
    } // namespace facebook
    ```

  - Binary literals replaced by hex:
    ```c++
    // C++20
    static constexpr uint16_t kHandleTypeMask = 0b0000'0000'0000'0111;
    static constexpr uint16_t kHandleIndexedMask = 0b0000'0000'0000'1000;
    static constexpr uint16_t kHandleValueMask = 0b1111'1111'1111'0000;

    // C++11
    static constexpr uint16_t kHandleTypeMask    = 0x0007; // 0b0000'0000'0000'0111;
    static constexpr uint16_t kHandleIndexedMask = 0x0008; // 0b0000'0000'0000'1000;
    static constexpr uint16_t kHandleValueMask   = 0xFFF0; // 0b1111'1111'1111'0000;
    ```

  - Instead of [std::bit_cast()](https://en.cppreference.com/w/cpp/numeric/bit_cast),
    [std::bit_width()](https://en.cppreference.com/w/cpp/numeric/bit_width),
    [std::make_unique()](https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique),
    [std::equal()](https://en.cppreference.com/w/cpp/algorithm/equal)
    used C++11-compatible analogues. See [/yoga/compat/](./yoga/compat/)

  - Added additional curly braces in places where arrays initialized:
    ```c++
    // C++20
    std::array<uint32_t, BufferSize> buffer_{};

    // C++11
    std::array<uint32_t, BufferSize> buffer_{{}};
    ```

  - Removed `noexcept` specifier. Have no idea, why we cant have it here in C++11:
    ```c++
    // C++20
    Node& operator=(Node&&) noexcept = default;

    // C++11
    Node& operator=(Node&&) = default;
    ```

  - `constexpr` functions
      - Changed implementation of some `constexpr` functions/methods to be compatible with C++11
        (for example: in C++11 we cannot use `switch` statement):
        ```c++
        // C++20
        constexpr FloatOptional resolve(float referenceLength) {
          switch (unit_) {
            case Unit::Point:
              return value_;
            case Unit::Percent:
              return FloatOptional{value_.unwrap() * referenceLength * 0.01f};
            default:
              return FloatOptional{};
          }
        }

        // C++11
        constexpr FloatOptional resolve(float referenceLength) {
          return
            (unit_ == Unit::Point) ?
              value_
            : (unit_ == Unit::Percent) ?
              FloatOptional{value_.unwrap() * referenceLength * 0.01f}
            :
              FloatOptional{}; // default
        }
        ```

      - `constexpr void` methods changed into `inline void`, since it not allowed in C++11:
        ```c++
        // C++20
        constexpr void setType(Type handleType) {
          repr_ &= (~kHandleTypeMask);
          repr_ |= static_cast<uint8_t>(handleType);
        }

        // C++11
        inline void setType(Type handleType) {
          repr_ &= (~kHandleTypeMask);
          repr_ |= static_cast<uint8_t>(handleType);
        }
        ```

      - Some `constexpr` functions with variables declaration in body (its not allowed in C++11)
        changed into `inline` , since I'm too lazy to replace them by `constexpr` implementation,
        compatible with C++11 :D
        ```c++
        // C++20
        static constexpr uint16_t packInlineInteger(float value) {
          uint16_t isNegative = value < 0 ? 1 : 0;
          return static_cast<uint16_t>(
              (isNegative << 11) |
              (static_cast<int32_t>(value) * (isNegative != 0u ? -1 : 1)));
        }

        // C++11
        static inline uint16_t packInlineInteger(float value) {
          uint16_t isNegative = value < 0 ? 1 : 0;
          return static_cast<uint16_t>(
              (isNegative << 11) |
              (static_cast<int32_t>(value) * (isNegative != 0u ? -1 : 1)));
        }
        ```

  - Concepts
      - Concepts was replaced by `static_assert()`s inside functions:
        ```c++
        // C++20
        template <typename EnumT>
        concept HasOrdinality = (ordinalCount<EnumT>() > 0);

        template <HasOrdinality EnumT>
        constexpr int32_t bitCount() {
          return std::bit_width( static_cast< std::underlying_type_t<EnumT> >(ordinalCount<EnumT>() - 1) );
        }

        // C++11
        template <typename EnumT>
        constexpr bool HasOrdinality() { return ordinalCount<EnumT>() > 0; }

        template <typename EnumT>
        constexpr int32_t bitCount() {
          static_assert(HasOrdinality<EnumT>() == true, "EnumT not has ordinality");
          return compat::bit_width( static_cast< typename std::underlying_type<EnumT>::type >(ordinalCount<EnumT>() - 1) );
        }
        ```
        ```c++
        // C++20
        constexpr bool isUndefined(std::floating_point auto value) {
          return value != value;
        }

        // C++11
        template <typename T>
        constexpr bool isUndefined(T value) {
          static_assert(std::is_floating_point<T>::value == true, "T must be floating-point");
          return value != value;
        }
        ```

      - Some concepts too hard to properly implement in C++11, so single usage of
        [std::input_iterator](https://en.cppreference.com/w/cpp/iterator/input_iterator)
        was simply commented :D
        ```c++
        // C++20
        static_assert(std::input_iterator<LayoutableChildren<T>::Iterator>);

        // C++11
        //static_assert(std::input_iterator<LayoutableChildren<T>::Iterator>); // <-- FIXME: TOO HARD TO EXPRESS IT IN C++11
        ```

  - Operators
      - Added `operator !=` for a few structures, when it called somewhere in code.
        In C++20 compiler generates it implicitly, if we provide `operator ==`,
        but in C++11 we must write it explicitly:

        ```c++
        // C++20
        constexpr bool operator==(const StyleLength& rhs) const {
          return value_ == rhs.value_ && unit_ == rhs.unit_;
        }

        // C++11
        constexpr bool operator==(const StyleLength& rhs) const {
          return value_ == rhs.value_ && unit_ == rhs.unit_;
        }

        // Added explicit declaration and implementation
        constexpr bool operator!=(const StyleLength& rhs) const {
          return !(*this == rhs);
        }
        ```

      - `default` comparison operators replaced by explicit implementation:
        ```c++
        // C++20
        bool operator==(const Iterator& other) const = default;
        bool operator!=(const Iterator& other) const = default;

        // C++11
        bool operator==(const Iterator& other) const { return this->e == other.e; }
        bool operator!=(const Iterator& other) const { return this->e != other.e; }
        ```

  - [Designated initializers](https://en.cppreference.com/w/cpp/language/aggregate_initialization#Designated_initializers)
    replaced by simple initialization:
    ```c++
    // C++20
    return FlexLine{
        .itemsInFlow = std::move(itemsInFlow),
        .sizeConsumed = sizeConsumed,
        .numberOfAutoMargins = numberOfAutoMargins,
        .layout = FlexLineRunningLayout{
            totalFlexGrowFactors,
            totalFlexShrinkScaledFactors,
        }};

    // C++11
    // Added constructor
    FlexLine(
      std::vector<yoga::Node*>&& itemsInFlow_,
      float sizeConsumed_,
      size_t numberOfAutoMargins_,
      const FlexLineRunningLayout& layout_
    )
      : itemsInFlow( std::move(itemsInFlow_) )
      , sizeConsumed(sizeConsumed_)
      , numberOfAutoMargins(numberOfAutoMargins_)
      , layout(layout_)
    {}

    // Constructor call
    return FlexLine{
      std::move(itemsInFlow), // itemsInFlow
      sizeConsumed,           // sizeConsumed
      numberOfAutoMargins,    // numberOfAutoMargins
      FlexLineRunningLayout{  // layout
          totalFlexGrowFactors,
          totalFlexShrinkScaledFactors,
      }};
    ```
    ```c++
    // C++20
    return {
        .width = maxOrDefined(0.0f, size.width),
        .height = maxOrDefined(0.0f, size.height)};

    // C++11
    return {
        maxOrDefined(0.0f, size.width), // width
        maxOrDefined(0.0f, size.height) // height
    };
    ```

  - `auto` in templates replaced by explicit types:
    ```c++
    // C++20
    template <auto LayoutMember>
    float getResolvedLayoutProperty(const YGNodeConstRef nodeRef, const Edge edge)

    // C++11
    template <float (LayoutResults::*LayoutMember)(PhysicalEdge) const>
    float getResolvedLayoutProperty(const YGNodeConstRef nodeRef, const Edge edge)
    ```
    ```c++
    // C++20
    template <auto GetterT, auto SetterT, typename IdxT, typename ValueT>
    void updateStyle(YGNodeRef node, IdxT idx, ValueT value)

    // C++11
    template <
      typename IdxT,
      typename ValueT,
      ValueT (Style::*GetterT)(IdxT) const,
      void (Style::*SetterT)(IdxT, ValueT)
    >
    void updateStyle(YGNodeRef node, IdxT idx, ValueT value)

    // ---------------------------------------------------------------------

    // C++20
    updateStyle<&Style::margin, &Style::setMargin>(...)

    // C++11
    updateStyle<Edge, Style::Length, &Style::margin, &Style::setMargin>(...)
    ```

  - Member bit-fields changes
    ```c++
    // C++20
    Direction direction_ : bitCount<Direction>() = Direction::Inherit;
    bool hadOverflow_ : 1 = false;
    ```

    In C++11 we have the next restrictions:
    1. We cannot initialize bit-fields in-place, only in constructor.
    2. We cannot use scoped enums with restricted bit count.

    That's why in C++11 we have the next boilerplate:

    ```c++
    // C++11

    // Member bit-fields initialization in constructor
    LayoutResults()
      : direction_{yoga::to_underlying(Direction::Inherit)} // Raw value initialized from scoped enum value
      , hadOverflow_{false}
    {}

    ...

    Direction direction() const {
      return static_cast<Direction>(direction_); // Added conversion: raw value --> scoped enum
    }

    void setDirection(Direction direction) {
      direction_ = yoga::to_underlying(direction); // Added conversion: scoped enum value --> raw value
    }

    ...

    // Raw type instead scoped enum as bit-field
    typename std::underlying_type<Direction>::type direction_ : bitCount<Direction>(); // Default: Direction::Inherit
    bool hadOverflow_ : 1; // Default: false
    ```

    Detailed description - why we use as bit-field value type 'raw type' instead of 'scoped enum type'

    In the next cases:
      ```c++
      Direction direction_ : bitCount<Direction>(); // Default: Direction::Inherit
      ```
    old compilers (like `gcc 4.9.2`) produces the next warning:

      ```
      'facebook::yoga::LayoutResults::direction_' is too small to hold all values of 'enum class facebook::yoga::Direction'
      ```

    Types info:
      ```c++
      enum class Direction : uint8_t {
        Inherit = YGDirectionInherit, // 0
        LTR = YGDirectionLTR,         // 1
        RTL = YGDirectionRTL,         // 2
      };

      template <>
      constexpr int32_t ordinalCount<Direction>() {
        return 3;
      }
      ```

    So `bitCount<Direction>()` returns `2`, because we have 3 enumerators and
    `bit_width(3 - 1)` = `bit_width(2)` = 2 bits.

    However, an `enum class Direction : uint8_t` **promises it can hold _any_ value
    representable by** `uint8_t`, i.e. `0..255`. By saying `Direction direction_ : 2;`,
    we're telling the compiler you only have 2 bits of storage - but the full range
    of `uint8_t` (0..255) obviously won't fit in 2 bits. That discrepancy triggers the
    "too small to hold all values" warning/error.

    Possible solutions to fix it:

    1. Using enum's `underlying_type` (raw type) instead:

        ```c++
        typename std::undrlying_type<Direction>::type direction_ : bitCount<Direction>();
        ```

       This satisfies the compiler that `direction_` is just a 2-bit integer. But we
       control how it maps to/from the `enum class Direction`.

    2. Stop forcing `Direction` to have an 8-bit underlying type, by declaring:

        ```c++
        enum class Direction { ... };
        ```

       the compiler might still give `int` storage by default - but at least we're no
       longer guaranteeing it's an 8-bit type with a 0..255 range. Some compilers
       could store it in fewer bits internally, though that's not guaranteed. Even
       then, a bit-field of type `Direction` can still trigger warnings unless the
       compiler sees that the enum range fits exactly in the specified bits.

    3. Use a normal member, not a bit-field:

        ```c++
        Direction direction_ = Direction::Inherit;
        ```

       But without a bit-field we lose the space optimization, even the warning goes
       away.

    **Choosed solution** - we use scoped enum's `underlying_type` (raw type) as bit-field value type.

</details>

## Building

To build the main library and run unit tests provided a wrapper:

```sh
./unit_tests <Debug|Release>
```

While not required, this script will use [ninja](https://ninja-build.org/) if it is installed for faster builds.

Yoga is additionally part of the [vcpkg](https://github.com/Microsoft/vcpkg/) collection of ports maintained by Microsoft and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/Microsoft/vcpkg) on the vcpkg repository.

## Adding Tests

Many of Yoga's tests are automatically generated, using HTML fixtures describing node structure. These are rendered in Chrome to generate an expected layout result for the tree. New fixtures can be added to `gentest/fixtures`.

```html
<div id="my_test" style="width: 100px; height: 100px; align-items: center;">
  <div style="width: 50px; height: 50px;"></div>
</div>
```

To generate new tests from added fixtures:

1. Ensure you have [yarn classic](https://classic.yarnpkg.com) installed.
2. Run `yarn install` to install dependencies for the test generator.
3. Run `yarn gentest` in the `yoga` directory.

## Debugging

Yoga provides a VSCode "launch.json" configuration which allows debugging unit tests. Simply add your breakpoints, and run "Debug C++ Unit tests (lldb)" (or "Debug C++ Unit tests (vsdbg)" on Windows).
