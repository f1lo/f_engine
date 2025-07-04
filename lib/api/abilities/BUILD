load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "ability",
    srcs = ["ability.cc"],
    hdrs = ["ability.h"],
    deps = [
        "//lib/api/abilities:keys",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//raylib",
        "@abseil-cpp//absl/log",
    ],
)

cc_library(
    name = "click_ability",
    srcs = ["click_ability.cc"],
    hdrs = ["click_ability.h"],
    deps = [
        "//lib/api/abilities:ability",
        "//lib/api/objects:object",
        "//lib/api/objects:static_object",
        "//raylib",
    ],
)

cc_library(
    name = "keys",
    hdrs = ["keys.h"],
    deps = [
        "//raylib",
    ],
)
