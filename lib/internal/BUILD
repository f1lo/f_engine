load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//api:__pkg__",
    "//examples:__pkg__",
])

cc_library(
    name = "hit_box",
    srcs = ["hit_box.cc"],
    hdrs = ["hit_box.h"],
    deps = [
        "//lib/internal/geometry:shape",
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/types:span",
    ],
)

cc_test(
    name = "hit_box_test",
    srcs = ["hit_box_test.cc"],
    deps = [
        ":hit_box",
        "//lib/internal/geometry:shape",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.h"],
    deps = [
        "//lib/internal/geometry:shape",
        "@abseil-cpp//absl/log:log",
    ],
)
