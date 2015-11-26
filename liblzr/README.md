liblzr
======

Library for handling laser graphics.

Inside, you will find the following tools:


ILDA File Parser
----------------

Simple parser designed to open `*.ild` laser graphics files. This parser will output `lzr_frame`s and `lzr_point`s. 3D points are projected orthographically (the Z coordinate is ignored, for now). No error correction is done. If a file is malformed, it will simply trigger an error and halt parsing.


Interpolator
------------

Simple tool for interpolating lit laser lines. **Note: the interpolator does not operate on blanked points, that's the optimizer's job**. There is a setting for the maximum distance between any two points on a lit line. If two adjacent points exceed this distance, new points are inserted. A custom interpolation function can also be supplied. The library defaults to `linear()`, but also has `quad()` and `quart()`.
