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
        "//lib/api/objects:screen_edge_object",
        "//raylib",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/memory",
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "level_test",
    srcs = ["level_test.cc"],
    deps = [
        ":level",
        "//lib/api/abilities:ability",
        "//lib/api/objects:static_object",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
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

cc_test(
    name = "title_screen_level_test",
    srcs = ["title_screen_level_test.cc"],
    deps = [
        ":level",
        ":title_screen_level",
        "//lib/api/abilities:ability",
        "//lib/api/objects:static_object",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
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

cc_test(
    name = "common_types_test",
    srcs = ["common_types_test.cc"],
    deps = [
        ":common_types",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
