load("@rules_cc//cc:defs.bzl", "cc_import")

cc_import(
    name = "raylib",
    hdrs = [
        "include/raylib.h",
        "include/raymath.h",
        "include/rlgl.h",
    ],
    static_library = "//raylib/linux_static:libraylib.a",
    visibility = ["//visibility:public"],
)
