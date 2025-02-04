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
        "//lib/internal:vertex",
        "@abseil-cpp//absl/status:statusor",
    ],
)

cc_test(
    name = "hit_box_test",
    srcs = ["hit_box_test.cc"],
    deps = [
        ":hit_box",
        ":vertex",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.h"],
    deps = [
        "//lib/internal:vertex",
        "@abseil-cpp//absl/log:log",
    ],
)

cc_library(
    name = "vertex",
    srcs = ["vertex.cc"],
    hdrs = ["vertex.h"],
)

cc_test(
    name = "vertex_test",
    srcs = ["vertex_test.cc"],
    deps = [
        ":vertex",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
