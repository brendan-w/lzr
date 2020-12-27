load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "etherdream",
    srcs=[
        "etherdream.c",
        "protocol.h",
    ],
    hdrs = [
        "etherdream.h",
    ],
    includes = [
        "./",  # libetherdream has a system include for <protocol.h>
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)