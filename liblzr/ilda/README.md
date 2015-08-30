ILDA File Parser
================

Simple parser designed to open `*.ild` laser graphics files. This parser will output `lzr_frame`s and `lzr_point`s. 3D points are projected orthographically (the Z coordinate is ignored, for now). No error correction is done. If a file is malformed, it will simply trigger an error and halt parsing.
