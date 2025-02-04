load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "simple_window",
    srcs = ["simple_window.cc"],
    deps = [
        "//raylib",
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//lib/internal:object",
        "//lib/internal:vertex",
    ],
)
