load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//lib/api:__subpackages__",
])

cc_library(
    name = "shader_internal",
    srcs = ["shader_internal.cc"],
    hdrs = ["shader_internal.h"],
    data = glob(["resources/**"]),
    deps = [
        "//raylib",
        "@abseil-cpp//absl/hash",
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/strings",
    ],
)

cc_library(
    name = "shader_internal_factory",
    srcs = ["shader_internal_factory.cc"],
    hdrs = ["shader_internal_factory.h"],
    deps = [
        ":shader_internal",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/memory",
    ],
)

cc_test(
    name = "shader_test",
    srcs = ["shader_test.cc"],
    deps = [
        ":shader_internal",
        ":shader_internal_factory",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
