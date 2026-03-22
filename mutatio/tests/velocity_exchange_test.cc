#include "mutatio/velocity_exchange.h"

#include <cmath>

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/location_view.h"
#include "mutatio/location_view_exchange.h"
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

// AerVelocity two-location conversions.
// All tests use (0°N, 0°E) as origin_loc unless otherwise noted.
// At that location: North = +Z ECEF, East = +Y ECEF, Up = +X ECEF (Down = -X).

TEST(VelocityExchange, AerVelocityFromRangeRate) {
  // Target due north at range 1000 m, elevation 0. Pure range rate (moving
  // directly away). At (0°N, 0°E): north = +Z ECEF, so vnorth=vrange.
  const LlaLocation origin{0.0, 0.0, 0.0};
  // ~0.009° ≈ 1000 m north at the equator.
  const LlaLocation point{0.009, 0.0, 0.0};

  const AerVelocity vel{0.0, 0.0, 1.0};  // vrange = 1 m/s
  NedVelocity result;
  auto stat = VelocityFrom(origin, point, vel, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  // Pure range rate along north direction → vnorth ≈ 1, veast ≈ 0, vdown ≈ 0.
  ASSERT_NEAR(result.vnorth, 1.0, 1e-6);
  ASSERT_NEAR(result.veast, 0.0, 1e-6);
  ASSERT_NEAR(result.vdown, 0.0, 1e-6);
}

TEST(VelocityExchange, AerVelocityFromAzimuthRate) {
  // Target due north at range r, elevation 0. Azimuth rate = 1 deg/s.
  // The lateral velocity at az=0 is veast = r * sin(1°) ≈ r * deg rad/s
  // For az=0, el=0: veast = r * cos(0) * vazimuth_rad = r * deg.
  const LlaLocation origin{0.0, 0.0, 0.0};
  const LlaLocation point{0.009, 0.0, 0.0};

  // Get the actual range to compute the expected veast.
  AerLocationView aer_pos;
  ViewFrom(origin, point, &aer_pos);
  const double r   = aer_pos.range;
  const double deg = M_PI / 180.0;

  const AerVelocity vel{1.0, 0.0, 0.0};  // vazimuth = 1 deg/s
  NedVelocity result;
  auto stat = VelocityFrom(origin, point, vel, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  // az=0, el=0: vnorth ≈ 0, veast = r*deg, vdown ≈ 0
  ASSERT_NEAR(result.vnorth, 0.0, 1e-6);
  ASSERT_NEAR(result.veast, r * deg, 1e-6);
  ASSERT_NEAR(result.vdown, 0.0, 1e-6);
}

TEST(VelocityExchange, AerVelocityFromElevationRate) {
  // Target due north at range r, elevation 0. Elevation rate = 1 deg/s.
  // For az=0, el=0: vdown = -r * cos(0) * velevation_rad = -r * deg.
  const LlaLocation origin{0.0, 0.0, 0.0};
  const LlaLocation point{0.009, 0.0, 0.0};

  AerLocationView aer_pos;
  ViewFrom(origin, point, &aer_pos);
  const double r   = aer_pos.range;
  const double deg = M_PI / 180.0;

  const AerVelocity vel{0.0, 1.0, 0.0};  // velevation = 1 deg/s
  NedVelocity result;
  auto stat = VelocityFrom(origin, point, vel, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  // az=0, el=0: vnorth = 0, veast = 0, vdown = -r*deg
  ASSERT_NEAR(result.vnorth, 0.0, 1e-6);
  ASSERT_NEAR(result.veast, 0.0, 1e-6);
  ASSERT_NEAR(result.vdown, -r * deg, 1e-6);
}

TEST(VelocityExchange, AerVelocityRoundTrip) {
  // AerVelocity → NedVelocity → AerVelocity should round-trip.
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 500.0};
  const AerVelocity original{2.5, -1.2, 50.0};

  auto ned = VelocityFrom<NedVelocity>(origin, point, original);
  AerVelocity reconstructed;
  auto stat = VelocityFrom(origin, point, ned, &reconstructed);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(reconstructed.vazimuth, original.vazimuth, 1e-9);
  ASSERT_NEAR(reconstructed.velevation, original.velevation, 1e-9);
  ASSERT_NEAR(reconstructed.vrange, original.vrange, 1e-9);
}

TEST(VelocityExchange, AerVelocityFromEcef) {
  // EcefVelocity → AerVelocity round-trip through NED at origin_loc.
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 500.0};
  const NedVelocity ned_original{10.0, -5.0, 2.0};

  // NED → AER, then AER → ECEF → AER should reconstruct.
  AerVelocity aer_from_ned;
  auto stat = VelocityFrom(origin, point, ned_original, &aer_from_ned);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefVelocity ecef;
  stat = VelocityFrom(origin, point, aer_from_ned, &ecef);
  ASSERT_EQ(stat, Status::SUCCESS);

  AerVelocity reconstructed;
  stat = VelocityFrom(origin, point, ecef, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(reconstructed.vazimuth, aer_from_ned.vazimuth, 1e-9);
  ASSERT_NEAR(reconstructed.velevation, aer_from_ned.velevation, 1e-9);
  ASSERT_NEAR(reconstructed.vrange, aer_from_ned.vrange, 1e-9);
}

TEST(VelocityExchange, AerVelocityIdentity) {
  // AerVelocity → AerVelocity is a copy.
  const LlaLocation origin{33.0, 74.0, 0.0};
  const LlaLocation point{34.0, 75.0, 0.0};
  const AerVelocity vel{1.0, 2.0, 3.0};

  AerVelocity result;
  auto stat = VelocityFrom(origin, point, vel, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vazimuth, vel.vazimuth);
  ASSERT_DOUBLE_EQ(result.velevation, vel.velevation);
  ASSERT_DOUBLE_EQ(result.vrange, vel.vrange);
}

TEST(VelocityExchange, AerVelocitySingularRange) {
  // origin_loc == point_loc → range = 0 → inverse Jacobian is singular.
  const LlaLocation loc{33.0, 74.0, 0.0};
  const NedVelocity ned{1.0, 0.0, 0.0};
  AerVelocity result;
  auto stat = VelocityFrom(loc, loc, ned, &result);
  ASSERT_EQ(stat, Status::ERROR);
}

TEST(VelocityExchange, AerVelocitySingularElevation) {
  // Point directly above origin (elevation = 90°) → azimuth undefined → ERROR.
  const LlaLocation origin{0.0, 0.0, 0.0};
  const LlaLocation point_above{0.0, 0.0, 1000.0};  // 1000 m altitude above
  const NedVelocity ned{1.0, 0.0, 0.0};
  AerVelocity result;
  auto stat = VelocityFrom(origin, point_above, ned, &result);
  ASSERT_EQ(stat, Status::ERROR);
}

TEST(VelocityExchange, AerVelocitySingleLocReturnsError) {
  // Single-location AerVelocity dispatch always returns ERROR.
  const LlaLocation loc{33.0, 74.0, 0.0};
  const AerVelocity aer_vel{1.0, 0.0, 10.0};

  NedVelocity ned_out;
  ASSERT_EQ(VelocityFrom(loc, aer_vel, &ned_out), Status::ERROR);

  EcefVelocity ecef_out;
  ASSERT_EQ(VelocityFrom(loc, aer_vel, &ecef_out), Status::ERROR);

  NedVelocity ned_in{1.0, 0.0, 0.0};
  AerVelocity aer_out;
  ASSERT_EQ(VelocityFrom(loc, ned_in, &aer_out), Status::ERROR);
}

TEST(VelocityExchange, AerVelocityTwoLocVariantDispatch) {
  // VelocityTypes holding AerVelocity should work through two-location dispatch.
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 500.0};
  const AerVelocity aer_original{2.5, -1.2, 50.0};

  // Through variant.
  VelocityTypes vel_variant = aer_original;
  NedVelocity ned_from_variant;
  auto stat = VelocityFrom(origin, point, vel_variant, &ned_from_variant);
  ASSERT_EQ(stat, Status::SUCCESS);

  // Direct call should give the same result.
  NedVelocity ned_direct;
  VelocityFrom(origin, point, aer_original, &ned_direct);
  ASSERT_DOUBLE_EQ(ned_from_variant.vnorth, ned_direct.vnorth);
  ASSERT_DOUBLE_EQ(ned_from_variant.veast, ned_direct.veast);
  ASSERT_DOUBLE_EQ(ned_from_variant.vdown, ned_direct.vdown);
}

}  // namespace mutatio
