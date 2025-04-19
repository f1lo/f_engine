load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "ability",
    srcs = ["ability.cc"],
    hdrs = ["ability.h"],
    deps = [
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
    ],
)

cc_library(
    name = "keys",
    hdrs = ["keys.h"],
    deps = [
        "//raylib",
    ],
)
