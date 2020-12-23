load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "ilda",
    srcs = [
        "ilda.cpp",
        "ilda_utils.hpp",
        "ilda_palette.cpp",
        "ilda_write.cpp",
        "ilda_read.cpp",
    ],
    hdrs = [
        "ilda.hpp"
    ],
    deps=[
        "//liblzr/core",
        "//vendored:portable_endian",
    ],
    visibility = ["//visibility:public"],
)