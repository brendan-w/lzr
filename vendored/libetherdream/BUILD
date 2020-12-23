load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "etherdream",
    srcs=[
        "etherdream.c",
    ],
    hdrs = [
        "etherdream.h",
        "protocol.h",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)