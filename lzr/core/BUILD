
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "core",
    srcs=[
        "frame.cpp",
        "interpolator.cpp",
        "mask.cpp",
        "point.cpp",
        "decimate.cpp",
        "transforms.cpp",
    ],
    hdrs = [
        "core.hpp"
    ],
    deps = [
        "@gsl",
    ],
    visibility = ["//visibility:public"],
)