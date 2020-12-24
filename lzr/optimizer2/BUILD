load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "optimizer2",
    srcs=[
        "optimizer2.cpp",
    ],
    hdrs = [
        "optimizer2.hpp"
    ],
    deps = [
        "//lzr/core",
        "@gtest//:gtest",
    ],
    visibility = ["//visibility:public"],
)