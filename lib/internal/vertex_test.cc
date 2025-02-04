#include "lib/internal/vertex.h"

#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

using ::lib::internal::Vertex;

TEST(VertexTest, IsLowerLeftEqualPointsReturnFalse) {
  const Vertex a{1, 2};
  const Vertex b{1, 2};

  EXPECT_FALSE(a.IsLowerLeft(b));
  EXPECT_FALSE(b.IsLowerLeft(a));
}

} // namespace
} // namespace internal
} // namespace lib
