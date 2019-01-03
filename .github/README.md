# cpp-object-pool

[![Release](https://img.shields.io/github/release/digimokan/cpp-object-pool.svg?label=release)](https://github.com/digimokan/cpp-object-pool/releases/latest "Latest Release Notes")
[![License](https://img.shields.io/badge/license-MIT-blue.svg?label=license)](LICENSE.txt "Project License")
[![Build Status](https://img.shields.io/travis/com/digimokan/cpp-object-pool/master.svg?label=linux+build)](https://travis-ci.com/digimokan/cpp-object-pool "Build And Test Results From Master Branch")

Type-integrated, O(1), aligned, thread-safe, stupid-simple C++ object pool
template.

## Table Of Contents

* [Motivation](#motivation)
* [Features](#features)
* [Requirements](#requirements)
* [Quick Start](#quick-start)
* [Options](#options)
* [Design Notes](#design-notes)
* [Contributing](#contributing)

## Motivation

Code an object pool from scratch, compliant with C++11. Make it efficient, but
also extremely simple to use.

## Features

* Ultra-fast setup using inherited, type-integrated template.
* O(1) allocation/deallocation of pooled objects.
* O(1) space complexity (no space required above space required for objects).
* Proper alignment of objects via use of `aligned_storage`.
* Thread-safe allocation/deallocation action.
* Pooled objects allocated/deallocated with normal `new` and `delete` calls.
* Automatic Pool creation/destruction.
* Fixed-size object pool.
* Pool statistics (e.g. peak pool size) available via inherited methods.

## Requirements

* C++11
* `std::thread`

## Quick Start

1. Copy the template source files from the [src](../src) directory into your
   project.

2. Include `PooledObject.hpp`:

   ```cpp
   #include "PooledObject.hpp"
   ```

3. Select the target class (`Foo`). Inherit from the object pool template. The
   template parameters are the name of the target class (repeated), and the size
   of the object pool (i.e. the number of `Foo` objects it will hold):

   ```cpp
   class Foo : public PooledObject<Foo, 10> {
   public:
     explicit Derived (int x) : x{x} { }
     int get_x () const { return x; }
   private:
     int x;
   };
   ```

4. The object pool is created automatically with the first use of a `new` call
   for `Foo`. Alternately, you can create the object pool ahead of time:

   ```cpp
   Foo::manual_allocate_object_pool();
   ```

5. Objects will be allocated/deallocated from the pool with normal `new` and
   `delete` calls:

   ```cpp
   Foo f = new Foo{4};
   delete f;
   ```

6. When `main` exits, the pool will be destroyed.

## Options

*  Get current peak size (the max number of objects that has been allocated at
   any point in time):

   ```cpp
   std::size_t peak_size = Foo::get_peak_size();
   ```

## Design Notes

* Overloads the type's `operator new` and `operator delete`.
* O(1) time complexity achieved by utilizing a singly-linked list for allocation
  and deallocation.
* O(1) space complexity achieved by utilizing a `union` for type.
* One-time-only pool creation realized via `std::call_once`.
* Pool destruction with program termination realized via `std::atexit`.
* A debug assert will be raised if an allocation would exceed the pool's fixed
  size.

## Contributing

* Feel free to report a bug or propose a feature by opening an issue.
* Follow the project's [Contributing](CONTRIBUTING.md) guidelines.
* Respect the project's [Code Of Conduct](CODE_OF_CONDUCT.md).

