#include "mutatio/velocity_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/status.h"
#include "mutatio/velocity.h"

namespace mutatio {

TEST(VelocityExchange, VelocityFromEcefIdentity) {
  EcefVelocity result;
  auto stat = VelocityFrom(EcefVelocity{1.0, 2.0, 3.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 1.0);
  ASSERT_DOUBLE_EQ(result.vy, 2.0);
  ASSERT_DOUBLE_EQ(result.vz, 3.0);
}

TEST(VelocityExchange, VelocityFromVariants) {
  VelocityTypes vel_variant = EcefVelocity{1.0, 2.0, 3.0};

  EcefVelocity result;
  auto stat = VelocityFrom(vel_variant, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 1.0);
  ASSERT_DOUBLE_EQ(result.vy, 2.0);
  ASSERT_DOUBLE_EQ(result.vz, 3.0);
}

TEST(VelocityExchange, ThrowingWrapper) {
  auto result = VelocityFrom<EcefVelocity>(EcefVelocity{1.0, 2.0, 3.0});
  ASSERT_DOUBLE_EQ(result.vx, 1.0);
  ASSERT_DOUBLE_EQ(result.vy, 2.0);
  ASSERT_DOUBLE_EQ(result.vz, 3.0);
}

}  // namespace mutatio
