load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "ability",
    srcs = ["ability.cc"],
    hdrs = ["ability.h"],
    deps = [
        "//lib/api:camera",
        "//lib/api/abilities:keys",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//raylib",
        "@abseil-cpp//absl/log",
    ],
)

cc_test(
    name = "move_ability_test",
    srcs = ["move_ability_test.cc"],
    deps = [
        ":ability",
        "//lib/api:camera",
        "//lib/api:common_types",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:static_object",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "click_ability",
    srcs = ["click_ability.cc"],
    hdrs = ["click_ability.h"],
    deps = [
        "//lib/api:camera",
        "//lib/api:common_types",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:keys",
        "//lib/api/objects:object",
        "//lib/api/objects:static_object",
    ],
)

cc_library(
    name = "move_with_cursor_ability",
    srcs = ["move_with_cursor_ability.cc"],
    hdrs = ["move_with_cursor_ability.h"],
    deps = [
        "//lib/api:camera",
        "//lib/api:common_types",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:keys",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:static_object",
    ],
)

cc_library(
    name = "projectile_ability",
    srcs = ["projectile_ability.cc"],
    hdrs = ["projectile_ability.h"],
    deps = [
        "//lib/api:camera",
        "//lib/api:common_types",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:keys",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:projectile_object",
        "//raylib",
        "@abseil-cpp//absl/container:flat_hash_set",
    ],
)

cc_library(
    name = "keys",
    hdrs = ["keys.h"],
    deps = [
        "//lib/api:common_types",
        "//raylib",
    ],
)
