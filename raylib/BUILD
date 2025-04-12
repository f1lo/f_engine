load("@rules_cc//cc:defs.bzl", "cc_import")

alias(
    name = "raylib",
    actual = select({
        "@platforms//os:linux": ":raylib_linux",
        "@platforms//os:windows": ":raylib_windows",
    }),
    visibility = ["//visibility:public"],
)

cc_import(
    name = "raylib_linux",
    hdrs = [
        "include/raylib.h",
        "include/raymath.h",
        "include/rlgl.h",
    ],
    static_library = select({
        "@platforms//os:linux": "//raylib/linux_static:libraylib.a",
    }),
    visibility = ["//visibility:private"],
    alwayslink = True,
)

cc_import(
    name = "raylib_windows",
    hdrs = [
        "include/raylib.h",
        "include/raymath.h",
        "include/rlgl.h",
    ],
    interface_library = "//raylib/win_static:raylibdll.lib",
    shared_library = "//raylib/win_static:raylib.dll",
    visibility = ["//visibility:private"],
    alwayslink = True,
)
