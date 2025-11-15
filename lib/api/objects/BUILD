load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api/sprites:sprite_instance",
        "//lib/internal:hit_box",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:any_invocable",
    ],
)

cc_test(
    name = "object_test",
    srcs = ["object_test.cc"],
    deps = [
        ":object",
        ":object_utils",
        "//lib/api:common_types",
        "//lib/internal:hit_box",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "movable_object",
    srcs = ["movable_object.cc"],
    hdrs = ["movable_object.h"],
    deps = [
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
        "//lib/api/sprites:sprite_instance",
        "//lib/internal/geometry:vec",
    ],
)

cc_test(
    name = "movable_object_test",
    srcs = ["movable_object_test.cc"],
    deps = [
        ":movable_object",
        "//lib/api:common_types",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "object_utils",
    srcs = ["object_utils.cc"],
    hdrs = ["object_utils.h"],
    visibility = ["//visibility:private"],
    deps = [
        "//lib/api:common_types",
        "//lib/api/objects:object",
        "//lib/internal:hit_box",
        "//lib/internal/geometry:shape",
        "//raylib",
        "@abseil-cpp//absl/log:check",
    ],
)

cc_test(
    name = "object_utils_test",
    srcs = ["object_utils_test.cc"],
    deps = [
        ":object_utils",
        "//lib/api:common_types",
        "//lib/internal:hit_box",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "static_object",
    srcs = ["static_object.cc"],
    hdrs = ["static_object.h"],
    deps = [
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
        "//lib/api/sprites:sprite_instance",
        "@abseil-cpp//absl/container:flat_hash_map",
    ],
)

cc_test(
    name = "static_object_test",
    srcs = ["static_object_test.cc"],
    deps = [
        ":static_object",
        "//lib/api:graphics_mock",
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
        "//lib/api/sprites:sprite_factory",
        "//lib/api/sprites:sprite_instance",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "projectile_object",
    srcs = ["projectile_object.cc"],
    hdrs = ["projectile_object.h"],
    deps = [
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/sprites:sprite_instance",
        "@abseil-cpp//absl/container:flat_hash_set",
    ],
)

cc_test(
    name = "projectile_object_test",
    srcs = ["projectile_object_test.cc"],
    deps = [
        ":projectile_object",
        ":static_object",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "screen_edge_object",
    srcs = ["screen_edge_object.cc"],
    hdrs = ["screen_edge_object.h"],
    deps = [
        "//lib/api:common_types",
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
        "@abseil-cpp//absl/log:log",
        "@abseil-cpp//absl/memory",
    ],
)

cc_test(
    name = "screen_edge_object_test",
    srcs = ["screen_edge_object_test.cc"],
    deps = [
        ":screen_edge_object",
        "//lib/api:common_types",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "coordinate_object",
    srcs = ["coordinate_object.cc"],
    hdrs = ["coordinate_object.h"],
    deps = [
        ":object_utils",
        "//lib/api:common_types",
        "//lib/api/objects:object",
        "//raylib",
        "@abseil-cpp//absl/memory",
    ],
)

cc_test(
    name = "coordinate_object_test",
    srcs = ["coordinate_object_test.cc"],
    deps = [
        ":coordinate_object",
        ":object",
        "//lib/api:common_types",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
