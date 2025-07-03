#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

ABSL_FLAG(bool, is_debug_mode, false, "Run game in debug mode.");

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);

  return 0;
}