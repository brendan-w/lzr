load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

# locally install SDL2 install
# TODO(bwhitfield): Formalize this
new_local_repository(
    name = "sdl2",
    path = "/usr/include/SDL2",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "sdl2",
    hdrs = glob(["**/*.h"])
)
"""
)

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    shallow_since = "1570114335 -0400",
)

new_git_repository(
    name = "nanosvg",
    remote = "https://github.com/memononen/nanosvg",
    commit = "3e403ec72a9145cbbcc6c63d94a4caf079aafec2",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "nanosvg",
    hdrs = [
        "src/nanosvg.h",
        "src/nanosvgrast.h",
    ],
    strip_include_prefix = "src/",
)
"""
)

new_git_repository(
    name = "gsl",
    remote = "https://github.com/microsoft/GSL",
    commit = "0f6dbc9e2915ef5c16830f3fa3565738de2a9230",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "gsl",
    hdrs = glob(["include/gsl/*"]),
    strip_include_prefix = "include/",
)
"""
)
