
**DANGER** Heavy objects overhead

LZR
===

Open Source Laser Projection Software

LZR is currently collection of backend libraries and tools which can form the bones of laser show GUIs, or creative applications.

Contains:

- DAC drivers (currently only supports the [EtherDream DAC](http://www.ether-dream.com/), but implementations for other DACs are always welcome.)
- point/frame structures
- frame transformations
- frame interpolator
- frame optimizer
- ILDA file reader/writer

Building
========

Dependencies:

- C++17 compiler (g++)
- SDL 2.0

Very simple right now:

```shell
$ bazel build //...
```

This will build the library, as well as tools.

Example Commands
================

```shell
$ ./bazel-bin/tools/ilda-view share/ildatest.ild
```

Running the Tests
=================

```shell
$ bazel test //tests/...
```

License
=======

GNU LGPL v3
