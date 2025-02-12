load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "player_controllable_object",
    srcs = ["player_controllable_object.cc"],
    hdrs = ["player_controllable_object.h"],
    deps = [
        "//lib/internal:hit_box",
        "//lib/internal:object",
        "//lib/internal/geometry:shape",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status:statusor",
    ],
)

cc_library(
    name = "game_context",
    srcs = ["game_context.cc"],
    hdrs = ["game_context.h"],
    deps = [
        "//lib/internal:object",
        "//raylib",
        "@abseil-cpp//absl/log:check",
    ],
)
