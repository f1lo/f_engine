load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//examples/breakout:ball",
        "//examples/breakout:ball_ability",
        "//examples/breakout:brick",
        "//examples/breakout:level_main",
        "//examples/breakout:player_pad",
        "//lib/api:game",
        "//lib/api:level",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:controls",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:object_type",
        "//lib/api/objects:static_object",
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
        "//lib/api/objects:object_type",
    ],
)

cc_library(
    name = "ball",
    srcs = ["ball.cc"],
    hdrs = ["ball.h"],
    deps = [
        "//examples/breakout:brick",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:object_type",
        "//lib/api/objects:static_object",
    ],
)

cc_library(
    name = "ball_ability",
    srcs = ["ball_ability.cc"],
    hdrs = ["ball_ability.h"],
    deps = [
        "//examples/breakout:ball",
        "//examples/breakout:player_pad",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:controls",
    ],
)

cc_library(
    name = "brick",
    srcs = ["brick.cc"],
    hdrs = ["brick.h"],
    deps = [
        "//lib/api/objects:object",
        "//lib/api/objects:object_type",
        "//lib/api/objects:static_object",
    ],
)

cc_library(
    name = "level_main",
    srcs = ["level_main.cc"],
    hdrs = ["level_main.h"],
    deps = [
        "//examples/breakout:ball",
        "//lib/api:level",
    ],
)
