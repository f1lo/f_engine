load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "simple_window",
    srcs = ["simple_window.cc"],
    deps = [
        "//raylib",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:any_invocable",
        "@abseil-cpp//absl/strings",
    ],
)
