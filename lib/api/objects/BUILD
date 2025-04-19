load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.h"],
    deps = [
        "//lib/internal:hit_box",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:any_invocable",
    ],
)

cc_library(
    name = "player_controllable_object",
    srcs = ["player_controllable_object.cc"],
    hdrs = ["player_controllable_object.h"],
    deps = [
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
        "//lib/internal:hit_box",
        "//lib/internal/geometry:shape",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:any_invocable",
    ],
)

cc_library(
    name = "movable_object",
    srcs = ["movable_object.cc"],
    hdrs = ["movable_object.h"],
    deps = [
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
    ],
)

cc_library(
    name = "object_utils",
    srcs = ["object_utils.cc"],
    hdrs = ["object_utils.h"],
    visibility = ["//visibility:private"],
    deps = [
        "//lib/api/objects:object",
        "//lib/internal:hit_box",
        "//lib/internal/geometry:shape",
        "//raylib",
        "@abseil-cpp//absl/log:check",
    ],
)

cc_library(
    name = "static_object",
    srcs = ["static_object.cc"],
    hdrs = ["static_object.h"],
    deps = [
        "//lib/api/objects:object",
        "//lib/api/objects:object_utils",
        "@abseil-cpp//absl/container:flat_hash_map",
    ],
)
