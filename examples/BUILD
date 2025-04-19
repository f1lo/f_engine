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

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//lib/api:game",
        "//lib/api:level",
        "//lib/api/abilities:ability",
        "//lib/api/abilities:keys",
        "//lib/api/objects:movable_object",
        "//lib/api/objects:object",
        "//lib/api/objects:player_controllable_object",
    ],
)
