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