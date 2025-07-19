load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "game",
    srcs = ["game.cc"],
    hdrs = ["game.h"],
    deps = [
        "//lib/api:level",
        "//lib/api/objects:object",
        "//raylib",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/log:check",
    ],
)

cc_library(
    name = "level",
    srcs = ["level.cc"],
    hdrs = ["level.h"],
    deps = [
        "//lib/api:camera",
        "//lib/api/abilities:ability",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:static_object",
        "//raylib",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/log:check",
    ],
)

cc_library(
    name = "title_screen_level",
    srcs = ["title_screen_level.cc"],
    hdrs = ["title_screen_level.h"],
    deps = [
        "//lib/api:level",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:click_ability",
        "//lib/api/objects:object",
        "//lib/api/objects:static_object",
        "//raylib",
    ],
)

cc_library(
    name = "camera",
    srcs = ["camera.cc"],
    hdrs = ["camera.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api/objects:object",
        "//raylib",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/synchronization",
    ],
)

cc_library(
    name = "common_types",
    srcs = ["common_types.cc"],
    hdrs = ["common_types.h"],
    deps = [],
)
