
**DANGER** Heavy objects overhead

LZR
===

Open Source Laser Projection Software

LZR is currently a collection of backend libraries and binaries which can form the bones of laser show GUIs, or creative applications. There are three main components:

liblzr
------

Library containing basic laser utilities:

- point/frame structures
- frame transformations
- frame interpolator
- frame optimizer
- ILDA file reader/writer
- ZeroMQ frame transport

See [lzr.h](https://github.com/brendanwhitfield/lzr/blob/master/liblzr/lzr.h) for details.



lzrd
----

Laser daemon/driver accepting frames via ZeroMQ. This executable contains the DAC-specific code needed to send frames to the hardware, and is meant to provide a common interface to programs using `liblzr`. `lzrd` currently only supports the [EtherDream DAC](http://www.ether-dream.com/), but implementations for other DACs are always welcome.


lzrv
----
Projector preview application (SDL 2.0). This program will listen to the ZeroMQ data being sent to `lzrd`, and display it. This will allow you to work offline.




Building
========

Dependencies:

- C++11 compiler (g++)
- Make
- ZeroMQ 4.x
- SDL 2.0

Very simple right now:

```shell
$ make
```

This will produce the following binaries:

- liblzr/liblzr.so
- liblzr/liblzr.a
- lzrd/lzrd
- lzrv/lzrv


License
=======

GNU LGPL v3
