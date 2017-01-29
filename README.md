
**DANGER** Heavy objects overhead

LZR
===

Open Source Laser Projection Software

LZR is currently collection of backend libraries and binaries which can form the bones of laser show GUIs, or creative applications.

liblzr
------

Library containing basic laser utilities:

- DAC drivers (currently only supports the [EtherDream DAC](http://www.ether-dream.com/), but implementations for other DACs are always welcome.)
- point/frame structures
- frame transformations
- frame interpolator
- frame optimizer
- ILDA file reader/writer

See [liblzr.hpp](https://github.com/brendanwhitfield/lzr/blob/master/liblzr/liblzr.hpp) for details.

Building
========

Dependencies:

- C++11 compiler (g++)
- Make
- [nlohmann/json](https://github.com/nlohmann/json)
- SDL 2.0

Very simple right now:

```shell
$ make
```

This will produce the following binaries:

- liblzr/liblzr.so
- liblzr/liblzr.a
- lzrv/lzrv


License
=======

GNU LGPL v3
