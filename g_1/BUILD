load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//g_1:levels",
        "//lib/api:game",
        "//lib/api:level",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
    ],
)

cc_library(
    name = "levels",
    srcs = ["levels.cc"],
    hdrs = ["levels.h"],
    deps = [
        "//g_1:opening_level",
        "//lib/api:level",
        "//lib/api:title_screen_level",
        "//lib/api/objects:static_object",
    ],
)

cc_library(
    name = "opening_level",
    srcs = ["opening_level.cc"],
    hdrs = ["opening_level.h"],
    deps = [
        "//lib/api:level",
        "//lib/api/abilities:keys",
    ],
)
