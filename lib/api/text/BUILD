load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "f_font",
    srcs = ["f_font.cc"],
    hdrs = ["f_font.h"],
    deps = ["//raylib"],
)

cc_library(
    name = "font_factory",
    srcs = ["font_factory.cc"],
    hdrs = ["font_factory.h"],
    data = [
        "//lib/api/text/fonts:roboto_bold.ttf",
        "//lib/api/text/fonts:roboto_italic.ttf",
        "//lib/api/text/fonts:roboto_regular.ttf",
    ],
    deps = [
        ":f_font",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/log:log",
        "@abseil-cpp//absl/memory",
    ],
)

cc_library(
    name = "text",
    srcs = ["text.cc"],
    hdrs = ["text.h"],
    deps = [
        ":f_font",
        "//lib/api:common_types",
        "//raylib",
    ],
)
