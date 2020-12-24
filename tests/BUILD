load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_ilda",
    srcs = ["test_ilda.cpp"],
    deps = [
        "//lzr",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    data = [
        "//share:ildatest.ild",
    ]
)

cc_test(
    name = "test_interpolator",
    srcs = ["test_interpolator.cpp"],
    deps = [
        "//lzr",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "test_optimizer",
    srcs = ["test_optimizer.cpp"],
    deps = [
        "//lzr",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "test_optimizer2",
    srcs = ["test_optimizer2.cpp"],
    deps = [
        "//lzr",
        "//lzr/optimizer2",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)
