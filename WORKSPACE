load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

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