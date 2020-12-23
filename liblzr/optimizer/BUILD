load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "optimizer",
    srcs=[
        "optimizer.cpp",
        "optimizer_utils.hpp",
        "optimizer_compile_paths.cpp",
        "optimizer_find_paths.cpp",
        "optimizer_path.cpp",
        "optimizer_reorder_paths.cpp",
    ],
    hdrs = [
        "optimizer.hpp"
    ],
    deps = [
        "//liblzr/core",
    ],
    visibility = ["//visibility:public"],
)