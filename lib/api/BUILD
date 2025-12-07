load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "game",
    srcs = ["game.cc"],
    hdrs = ["game.h"],
    deps = [
        ":stats",
        "//lib/api:level",
        "//lib/api/objects:object",
        "//lib/api/objects:object_type",
        "//lib/api/sprites:sprite_factory",
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
        ":stats",
        "//lib/api:camera",
        "//lib/api:common_types",
        "//lib/api:controls",
        "//lib/api/abilities:ability",
        "//lib/api/objects:coordinate_object",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:object_type",
        "//lib/api/objects:screen_edge_object",
        "//lib/api/objects:static_object",
        "//lib/api/sprites:sprite",
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
        ":graphics_mock",
        ":level",
        "//lib/api:controls_mock",
        "//lib/api/abilities:ability",
        "//lib/api/objects:object_type",
        "//lib/api/objects:screen_edge_object",
        "//lib/api/objects:static_object",
        "//lib/api/sprites:sprite_factory",
        "//lib/api/sprites:sprite_instance",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "title_screen_level",
    srcs = ["title_screen_level.cc"],
    hdrs = ["title_screen_level.h"],
    deps = [
        "//lib/api:controls",
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
        "//lib/api/objects:object_type",
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
    ],
)

cc_library(
    name = "common_types",
    srcs = ["common_types.cc"],
    hdrs = ["common_types.h"],
    deps = [
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/strings",
    ],
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

cc_library(
    name = "graphics",
    srcs = ["graphics.cc"],
    hdrs = ["graphics.h"],
    deps = [
        "//raylib",
    ],
)

cc_library(
    name = "graphics_mock",
    srcs = ["graphics_mock.cc"],
    hdrs = ["graphics_mock.h"],
    deps = [
        ":graphics",
    ],
)

cc_library(
    name = "stats",
    srcs = ["stats.cc"],
    hdrs = ["stats.h"],
    deps = [
        "//lib/api/objects:object_type",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/time",
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "stats_test",
    srcs = ["stats_test.cc"],
    deps = [
        ":stats",
        "//lib/api/objects:object_type",
        "@abseil-cpp//absl/time",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "controls",
    srcs = ["controls.cc"],
    hdrs = ["controls.h"],
    deps = [
        ":camera",
        "//lib/api:common_types",
        "//raylib",
    ],
)

cc_library(
    name = "controls_mock",
    srcs = ["controls_mock.cc"],
    hdrs = ["controls_mock.h"],
    deps = [
        ":controls",
        "//lib/api:common_types",
    ],
)
