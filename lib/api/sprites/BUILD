load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "sprite",
    hdrs = ["sprite.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api:graphics",
    ],
)

cc_library(
    name = "static_sprite",
    srcs = ["static_sprite.cc"],
    hdrs = ["static_sprite.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api:graphics",
        "//lib/api/sprites:sprite",
        "//raylib",
    ],
)

cc_library(
    name = "animated_sprite",
    srcs = ["animated_sprite.cc"],
    hdrs = ["animated_sprite.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api:graphics",
        "//lib/api/sprites:sprite",
        "//raylib",
    ],
)

cc_library(
    name = "sprite_instance",
    srcs = ["sprite_instance.cc"],
    hdrs = ["sprite_instance.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api:graphics",
        "//lib/api/sprites:sprite",
        "@abseil-cpp//absl/time",
    ],
)

cc_library(
    name = "sprite_factory",
    srcs = ["sprite_factory.cc"],
    hdrs = ["sprite_factory.h"],
    deps = [
        ":animated_sprite",
        ":sprite",
        ":sprite_instance",
        ":static_sprite",
        "//lib/api:graphics",
        "//lib/api:graphics_mock",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/memory",
        "@abseil-cpp//absl/time",
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "sprite_test",
    srcs = ["sprite_test.cc"],
    deps = [
        ":animated_sprite",
        ":sprite",
        ":sprite_factory",
        ":sprite_instance",
        ":static_sprite",
        "//lib/api:common_types",
        "//lib/api:graphics",
        "//lib/api:graphics_mock",
        "//raylib",
        "@abseil-cpp//absl/time",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
