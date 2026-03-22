#include "mutatio/velocity_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/status.h"
#include "mutatio/velocity.h"

namespace mutatio {

TEST(VelocityExchange, VelocityFromEcefEcef) {
  EcefVelocity original{1.0, 2.0, 3.0};
  EcefVelocity copy;
  auto stat = VelocityFrom(original, &copy);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(original.vx, copy.vx);
  ASSERT_DOUBLE_EQ(original.vy, copy.vy);
  ASSERT_DOUBLE_EQ(original.vz, copy.vz);

  VelocityTypes variant = original;
  stat                  = VelocityFrom(variant, &copy);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(original.vx, copy.vx);
  ASSERT_DOUBLE_EQ(original.vy, copy.vy);
  ASSERT_DOUBLE_EQ(original.vz, copy.vz);

  copy = VelocityFrom<EcefVelocity>(original);
  ASSERT_DOUBLE_EQ(original.vx, copy.vx);
  ASSERT_DOUBLE_EQ(original.vy, copy.vy);
  ASSERT_DOUBLE_EQ(original.vz, copy.vz);
}

TEST(VelocityExchange, VelocityFromAssignment) {
  EcefVelocity original{4.0, 5.0, 6.0};
  VelocityTypes variant = original;
  auto copy             = VelocityFrom<EcefVelocity>(variant);

  ASSERT_DOUBLE_EQ(original.vx, copy.vx);
  ASSERT_DOUBLE_EQ(original.vy, copy.vy);
  ASSERT_DOUBLE_EQ(original.vz, copy.vz);
}

}  // namespace mutatio
