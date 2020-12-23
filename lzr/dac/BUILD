load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "dac",
    srcs=[
        "dac.cpp",
        "dac_utils.hpp",
        "etherdream.cpp",
        "etherdream.hpp",
    ],
    hdrs=[
        "dac.hpp",
    ],
    deps=[
        "//lzr/core",
        "//vendored/libetherdream:etherdream",
    ],
    visibility = ["//visibility:public"],
)