load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

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
    deps = [
        ":f_font",
        "@abseil-cpp//absl/container:flat_hash_map",
    ],
)
