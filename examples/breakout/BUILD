load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//examples/breakout:player_pad",
        "//lib/api:game",
        "//lib/api:level",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:keys",
        "//raylib",
    ],
)

cc_library(
    name = "player_pad",
    srcs = ["player_pad.cc"],
    hdrs = ["player_pad.h"],
    deps = [
        "//examples/breakout:ball",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
    ],
)

cc_library(
    name = "ball",
    srcs = ["ball.cc"],
    hdrs = ["ball.h"],
    deps = [
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
    ],
)
