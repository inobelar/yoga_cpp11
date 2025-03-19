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

Current state: based on [6455a84](https://github.com/facebook/yoga/commit/6455a848a76f433bdb48b2640b7f4644774c76fd) commit (Mar 11, 2025)

> [!NOTE]
> The main implementation targets `C++11`, but to build the unit tests
> (which rely on [GoogleTest](https://github.com/google/googletest)) needed
> `C++17` support.

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
