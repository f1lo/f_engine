load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//api:__pkg__",
    "//examples:__pkg__",
])

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.h"],
    deps = [
        "//lib/api:vertex",
        "@abseil-cpp//absl/log:log",
    ],
)
