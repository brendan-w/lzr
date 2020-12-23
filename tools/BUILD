load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "ilda-play",
    srcs=[
        "ilda_play.cpp",
    ],
    deps = [
        "//liblzr",
    ],
)

cc_binary(
    name = "ilda-view",
    srcs=[
        "ilda_view.cpp",
    	# "SDL2.h",
    ],
    deps = [
        "//liblzr",
        "@sdl2",
    ],
    copts = ["-Iexternal/sdl2"],
    linkopts = ["-lSDL2"],
)