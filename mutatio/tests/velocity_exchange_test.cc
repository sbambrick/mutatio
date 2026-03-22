#include "mutatio/velocity_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/status.h"
#include "mutatio/velocity.h"

namespace mutatio {

// At (0 N, 0 E): +X ECEF = away from Earth (up) = -down, +Y = east, +Z = north.
TEST(VelocityExchange, VelocityFromEcefNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity result;
  Status stat;

  stat = VelocityFrom(EcefVelocity{1.0, 0.0, 0.0}, loc, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);

  stat = VelocityFrom(EcefVelocity{0.0, 1.0, 0.0}, loc, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_DOUBLE_EQ(result.veast, 1.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);

  stat = VelocityFrom(EcefVelocity{0.0, 0.0, 1.0}, loc, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);

  result = VelocityFrom<NedVelocity>(EcefVelocity{1.0, 0.0, 0.0}, loc);
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);
}

TEST(VelocityExchange, VelocityFromNedEcef) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity result;
  Status stat;

  // north (+Z), east (+Y), down (-X) at (0 N, 0 E).
  stat = VelocityFrom(NedVelocity{1.0, 0.0, 0.0}, loc, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 0.0);
  ASSERT_DOUBLE_EQ(result.vy, 0.0);
  ASSERT_DOUBLE_EQ(result.vz, 1.0);

  stat = VelocityFrom(NedVelocity{0.0, 1.0, 0.0}, loc, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 0.0);
  ASSERT_DOUBLE_EQ(result.vy, 1.0);
  ASSERT_DOUBLE_EQ(result.vz, 0.0);

  stat = VelocityFrom(NedVelocity{0.0, 0.0, 1.0}, loc, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, -1.0);
  ASSERT_DOUBLE_EQ(result.vy, 0.0);
  ASSERT_DOUBLE_EQ(result.vz, 0.0);
}

TEST(VelocityExchange, VelocityFromVariants) {
  VelocityTypes vel_variant = EcefVelocity{0.0, 0.0, 1.0};
  LocationTypes loc_variant = LlaLocation{0.0, 0.0, 0.0};

  NedVelocity result;
  auto stat = VelocityFrom(vel_variant, loc_variant, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);
}

TEST(VelocityExchange, VelocityFromEcefNedRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity original{100.0, -50.0, 25.0};

  auto ned          = VelocityFrom<NedVelocity>(original, loc);
  auto reconstructed = VelocityFrom<EcefVelocity>(ned, loc);

  ASSERT_DOUBLE_EQ(reconstructed.vx, original.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, original.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, original.vz);
}

}  // namespace mutatio
