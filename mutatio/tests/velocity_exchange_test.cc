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

  stat = VelocityFrom(loc, EcefVelocity{1.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);

  stat = VelocityFrom(loc, EcefVelocity{0.0, 1.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_DOUBLE_EQ(result.veast, 1.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);

  stat = VelocityFrom(loc, EcefVelocity{0.0, 0.0, 1.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);

  result = VelocityFrom<NedVelocity>(loc, EcefVelocity{1.0, 0.0, 0.0});
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);
}

TEST(VelocityExchange, VelocityFromNedEcef) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity result;
  Status stat;

  // north (+Z), east (+Y), down (-X) at (0 N, 0 E).
  stat = VelocityFrom(loc, NedVelocity{1.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 0.0);
  ASSERT_DOUBLE_EQ(result.vy, 0.0);
  ASSERT_DOUBLE_EQ(result.vz, 1.0);

  stat = VelocityFrom(loc, NedVelocity{0.0, 1.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 0.0);
  ASSERT_DOUBLE_EQ(result.vy, 1.0);
  ASSERT_DOUBLE_EQ(result.vz, 0.0);

  stat = VelocityFrom(loc, NedVelocity{0.0, 0.0, 1.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, -1.0);
  ASSERT_DOUBLE_EQ(result.vy, 0.0);
  ASSERT_DOUBLE_EQ(result.vz, 0.0);
}

TEST(VelocityExchange, VelocityFromVariants) {
  VelocityTypes vel_variant = EcefVelocity{0.0, 0.0, 1.0};
  LocationTypes loc_variant = LlaLocation{0.0, 0.0, 0.0};

  NedVelocity result;
  auto stat = VelocityFrom(loc_variant, vel_variant, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);
}

TEST(VelocityExchange, VelocityFromEcefNedRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity original{100.0, -50.0, 25.0};

  auto ned           = VelocityFrom<NedVelocity>(loc, original);
  auto reconstructed = VelocityFrom<EcefVelocity>(loc, ned);

  ASSERT_DOUBLE_EQ(reconstructed.vx, original.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, original.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, original.vz);
}

TEST(VelocityExchange, VelocityFromSameType) {
  const LlaLocation loc{33.0, 74.0, 1000.0};

  EcefVelocity ecef_out;
  auto stat = VelocityFrom(loc, EcefVelocity{1.0, 2.0, 3.0}, &ecef_out);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(ecef_out.vx, 1.0);
  ASSERT_DOUBLE_EQ(ecef_out.vy, 2.0);
  ASSERT_DOUBLE_EQ(ecef_out.vz, 3.0);

  NedVelocity ned_out;
  stat = VelocityFrom(loc, NedVelocity{1.0, 2.0, 3.0}, &ned_out);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(ned_out.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(ned_out.veast, 2.0);
  ASSERT_DOUBLE_EQ(ned_out.vdown, 3.0);
}

TEST(VelocityExchange, VelocityFromVariantsSameType) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  VelocityTypes vel_variant = EcefVelocity{1.0, 2.0, 3.0};

  EcefVelocity result;
  auto stat = VelocityFrom(loc, vel_variant, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 1.0);
  ASSERT_DOUBLE_EQ(result.vy, 2.0);
  ASSERT_DOUBLE_EQ(result.vz, 3.0);
}

// Two-location VelocityFrom: origin_loc defines the NED frame of the input,
// point_loc defines the NED frame of the output.

TEST(VelocityExchange, VelocityFromTwoLocSameFrame) {
  // With identical locations the result is the same as the single-location API.
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity result;
  auto stat = VelocityFrom(loc, loc, EcefVelocity{0.0, 0.0, 1.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(result.veast, 0.0);
  ASSERT_DOUBLE_EQ(result.vdown, 0.0);
}

TEST(VelocityExchange, VelocityFromTwoLocEcefToNed) {
  // ECEF→NED uses point_loc only; origin_loc is unused.
  // +Y ECEF at (0°N, 90°E) is the outward radial = up, so vdown = -1.
  const LlaLocation origin_loc{0.0, 0.0, 0.0};
  const LlaLocation point_loc{0.0, 90.0, 0.0};
  NedVelocity result;
  auto stat = VelocityFrom(origin_loc, point_loc, EcefVelocity{0.0, 1.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_NEAR(result.veast, 0.0, 1e-15);
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);
}

TEST(VelocityExchange, VelocityFromTwoLocNedToEcef) {
  // NED→ECEF uses origin_loc only; point_loc is unused.
  // NED north at (0°N, 0°E) = +Z ECEF.
  const LlaLocation origin_loc{0.0, 0.0, 0.0};
  const LlaLocation point_loc{33.0, 74.0, 0.0};
  EcefVelocity result;
  auto stat = VelocityFrom(origin_loc, point_loc, NedVelocity{1.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 0.0);
  ASSERT_DOUBLE_EQ(result.vy, 0.0);
  ASSERT_DOUBLE_EQ(result.vz, 1.0);
}

TEST(VelocityExchange, VelocityFromTwoLocNedToNed) {
  // NED east {0,1,0} at (0°N, 0°E) = +Y ECEF.
  // At (0°N, 90°E), +Y ECEF is the outward radial = up → vdown = -1.
  const LlaLocation origin_loc{0.0, 0.0, 0.0};
  const LlaLocation point_loc{0.0, 90.0, 0.0};
  NedVelocity result;
  auto stat = VelocityFrom(origin_loc, point_loc, NedVelocity{0.0, 1.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_NEAR(result.veast, 0.0, 1e-15);
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);
}

TEST(VelocityExchange, VelocityFromTwoLocVariants) {
  const LocationTypes origin_loc = LlaLocation{0.0, 0.0, 0.0};
  const LocationTypes point_loc  = LlaLocation{0.0, 90.0, 0.0};
  VelocityTypes vel_variant      = NedVelocity{0.0, 1.0, 0.0};

  // NED east at (0°N, 0°E) = up at (0°N, 90°E).
  NedVelocity result;
  auto stat = VelocityFrom(origin_loc, point_loc, vel_variant, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vnorth, 0.0);
  ASSERT_NEAR(result.veast, 0.0, 1e-15);
  ASSERT_DOUBLE_EQ(result.vdown, -1.0);
}

}  // namespace mutatio
