load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//lib/internal:__pkg__",
])

cc_library(
    name = "vec",
    srcs = ["vec.cc"],
    hdrs = ["vec.h"],
)

cc_test(
    name = "vec_test",
    srcs = ["vec_test.cc"],
    deps = [
        ":vec",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "shape",
    srcs = ["shape.cc"],
    hdrs = ["shape.h"],
    deps = [
        ":vec",
        "@abseil-cpp//absl/log:log",
    ],
)

cc_test(
    name = "shape_test",
    srcs = ["shape_test.cc"],
    deps = [
        ":shape",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
