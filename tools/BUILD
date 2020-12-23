load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "ilda-play",
    srcs=[
        "ilda_play.cpp",
    ],
    deps = [
        "//lzr",
    ],
)

cc_binary(
    name = "ilda-view",
    srcs=[
        "ilda_view.cpp",
    ],
    deps = [
        "//lzr",
        "@sdl2",
    ],
    copts = ["-Iexternal/sdl2"],
    linkopts = ["-lSDL2"],
)

cc_binary(
    name = "ilda-from-svg",
    srcs=[
        "ilda_from_svg.cpp",
    ],
    deps = [
        "//lzr",
        "@nanosvg",
        "@gsl",
    ],
)
