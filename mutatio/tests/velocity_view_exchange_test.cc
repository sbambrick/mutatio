#include "mutatio/velocity_view_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/status.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

TEST(VelocityViewExchange, VelocityViewFromEcefEcef) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocity point{15.0, 18.0, 35.0};
  EcefVelocityView expected{5.0, -2.0, 5.0};

  EcefVelocityView view;
  auto stat = VelocityViewFrom(loc, origin, point, &view);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);

  view = VelocityViewFrom<EcefVelocityView>(loc, origin, point);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);
}

TEST(VelocityViewExchange, VelocityViewFromVariants) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  VelocityTypes origin = EcefVelocity{10.0, 20.0, 30.0};
  VelocityTypes point  = EcefVelocity{15.0, 18.0, 35.0};
  EcefVelocityView expected{5.0, -2.0, 5.0};

  EcefVelocityView view;
  auto stat = VelocityViewFrom(loc, origin, point, &view);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);
}

TEST(VelocityViewExchange, VelocityFromEcefView) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocityView view{5.0, -2.0, 5.0};
  EcefVelocity expected{15.0, 18.0, 35.0};

  EcefVelocity point;
  auto stat = VelocityFrom(loc, origin, view, &point);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, expected.vx);
  ASSERT_DOUBLE_EQ(point.vy, expected.vy);
  ASSERT_DOUBLE_EQ(point.vz, expected.vz);

  point = VelocityFrom<EcefVelocity>(loc, origin, view);
  ASSERT_DOUBLE_EQ(point.vx, expected.vx);
  ASSERT_DOUBLE_EQ(point.vy, expected.vy);
  ASSERT_DOUBLE_EQ(point.vz, expected.vz);
}

TEST(VelocityViewExchange, VelocityFromVariants) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  VelocityTypes origin           = EcefVelocity{10.0, 20.0, 30.0};
  VelocityViewTypes view_variant = EcefVelocityView{5.0, -2.0, 5.0};
  EcefVelocity expected{15.0, 18.0, 35.0};

  EcefVelocity point;
  auto stat = VelocityFrom(loc, origin, view_variant, &point);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, expected.vx);
  ASSERT_DOUBLE_EQ(point.vy, expected.vy);
  ASSERT_DOUBLE_EQ(point.vz, expected.vz);
}

TEST(VelocityViewExchange, RoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocity point{15.0, 18.0, 35.0};

  auto view          = VelocityViewFrom<EcefVelocityView>(loc, origin, point);
  auto reconstructed = VelocityFrom<EcefVelocity>(loc, origin, view);

  ASSERT_DOUBLE_EQ(reconstructed.vx, point.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, point.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, point.vz);
}

// At (0 N, 0 E): +Z ECEF = north, so a diff of (0,0,1) in ECEF -> (1,0,0) NED.
TEST(VelocityViewExchange, VelocityViewFromEcefNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocityView view;
  Status stat;

  stat = VelocityViewFrom(loc, EcefVelocity{0.0, 0.0, 1.0},
                          EcefVelocity{0.0, 0.0, 2.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);

  view = VelocityViewFrom<NedVelocityView>(loc, EcefVelocity{0.0, 0.0, 1.0},
                                           EcefVelocity{0.0, 0.0, 2.0});
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);
}

TEST(VelocityViewExchange, VelocityViewFromNedNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocityView view;

  auto stat = VelocityViewFrom(loc, NedVelocity{10.0, 20.0, 30.0},
                               NedVelocity{11.0, 22.0, 33.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 2.0);
  ASSERT_DOUBLE_EQ(view.vdown, 3.0);
}

TEST(VelocityViewExchange, VelocityFromEcefNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity point;

  // NED north at (0,0) = +Z ECEF, so origin{0,0,0} + view{1,0,0} = {0,0,1}.
  auto stat = VelocityFrom(loc, EcefVelocity{0.0, 0.0, 0.0},
                           NedVelocityView{1.0, 0.0, 0.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, 0.0);
  ASSERT_DOUBLE_EQ(point.vy, 0.0);
  ASSERT_DOUBLE_EQ(point.vz, 1.0);

  point = VelocityFrom<EcefVelocity>(loc, EcefVelocity{0.0, 0.0, 0.0},
                                     NedVelocityView{1.0, 0.0, 0.0});
  ASSERT_DOUBLE_EQ(point.vz, 1.0);
}

TEST(VelocityViewExchange, VelocityFromNedNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity point;

  auto stat = VelocityFrom(loc, NedVelocity{10.0, 20.0, 30.0},
                           NedVelocityView{1.0, 2.0, 3.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vnorth, 11.0);
  ASSERT_DOUBLE_EQ(point.veast, 22.0);
  ASSERT_DOUBLE_EQ(point.vdown, 33.0);
}

TEST(VelocityViewExchange, NedRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity origin{10.0, 20.0, 30.0};
  const EcefVelocity point{15.0, 18.0, 35.0};

  auto view          = VelocityViewFrom<NedVelocityView>(loc, origin, point);
  auto reconstructed = VelocityFrom<EcefVelocity>(loc, origin, view);

  ASSERT_DOUBLE_EQ(reconstructed.vx, point.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, point.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, point.vz);
}

TEST(VelocityViewExchange, NedVariantsRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  VelocityTypes origin = EcefVelocity{10.0, 20.0, 30.0};
  VelocityTypes point  = EcefVelocity{15.0, 18.0, 35.0};

  NedVelocityView view;
  auto stat = VelocityViewFrom(loc, origin, point, &view);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefVelocity reconstructed;
  stat = VelocityFrom(loc, origin, VelocityViewTypes{view}, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(reconstructed.vx, std::get<EcefVelocity>(point).vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, std::get<EcefVelocity>(point).vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, std::get<EcefVelocity>(point).vz);
}

// At (0 N, 0 E): +Z ECEF = north, +Y ECEF = east, +X ECEF = -down (up).
// A NED velocity of (1, 0, 0) = pure north = +Z in ECEF.
TEST(VelocityViewExchange, VelocityViewFromEcefNedEcefView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  // ECEF origin = (0,0,0), NED point = (1,0,0) north = (0,0,1) ECEF.
  // Expected EcefVelocityView = (0,0,1) - (0,0,0) = (0,0,1).
  EcefVelocityView view;
  auto stat = VelocityViewFrom(loc, EcefVelocity{0.0, 0.0, 0.0},
                               NedVelocity{1.0, 0.0, 0.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, 0.0);
  ASSERT_DOUBLE_EQ(view.vy, 0.0);
  ASSERT_DOUBLE_EQ(view.vz, 1.0);

  view = VelocityViewFrom<EcefVelocityView>(loc, EcefVelocity{0.0, 0.0, 0.0},
                                            NedVelocity{1.0, 0.0, 0.0});
  ASSERT_DOUBLE_EQ(view.vz, 1.0);
}

TEST(VelocityViewExchange, VelocityViewFromNedEcefEcefView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  // NED origin = (1,0,0) north = (0,0,1) ECEF, ECEF point = (0,0,2).
  // Expected EcefVelocityView = (0,0,2) - (0,0,1) = (0,0,1).
  EcefVelocityView view;
  auto stat = VelocityViewFrom(loc, NedVelocity{1.0, 0.0, 0.0},
                               EcefVelocity{0.0, 0.0, 2.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, 0.0);
  ASSERT_DOUBLE_EQ(view.vy, 0.0);
  ASSERT_DOUBLE_EQ(view.vz, 1.0);
}

TEST(VelocityViewExchange, VelocityViewFromNedNedEcefView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  // NED origin = (1,0,0) = (0,0,1) ECEF, NED point = (2,0,0) = (0,0,2) ECEF.
  // Expected EcefVelocityView = (0,0,2) - (0,0,1) = (0,0,1).
  EcefVelocityView view;
  auto stat = VelocityViewFrom(loc, NedVelocity{1.0, 0.0, 0.0},
                               NedVelocity{2.0, 0.0, 0.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, 0.0);
  ASSERT_DOUBLE_EQ(view.vy, 0.0);
  ASSERT_DOUBLE_EQ(view.vz, 1.0);
}

TEST(VelocityViewExchange, VelocityViewFromEcefNedNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  // ECEF origin = (0,0,1) = north at (0,0), NED point = (2,0,0) = (0,0,2) ECEF.
  // ECEF diff = (0,0,2) - (0,0,1) = (0,0,1) = 1 north in NED.
  NedVelocityView view;
  auto stat = VelocityViewFrom(loc, EcefVelocity{0.0, 0.0, 1.0},
                               NedVelocity{2.0, 0.0, 0.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);

  view = VelocityViewFrom<NedVelocityView>(loc, EcefVelocity{0.0, 0.0, 1.0},
                                           NedVelocity{2.0, 0.0, 0.0});
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
}

TEST(VelocityViewExchange, VelocityViewFromNedEcefNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  // NED origin = (1,0,0) = (0,0,1) ECEF, ECEF point = (0,0,2).
  // ECEF diff = (0,0,2) - (0,0,1) = (0,0,1) = 1 north in NED.
  NedVelocityView view;
  auto stat = VelocityViewFrom(loc, NedVelocity{1.0, 0.0, 0.0},
                               EcefVelocity{0.0, 0.0, 2.0}, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);
}

TEST(VelocityViewExchange, CrossTypeVariantsRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity ecef_origin{10.0, 20.0, 30.0};
  const NedVelocity ned_point{1.0, 2.0, 3.0};

  // Cross-type EcefVelocityView: reconstruct the ECEF point via origin + view.
  EcefVelocityView ecef_view;
  auto stat = VelocityViewFrom(loc, ecef_origin, ned_point, &ecef_view);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefVelocity reconstructed;
  stat = VelocityFrom(loc, ecef_origin, ecef_view, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  // Cross-type NedVelocityView path must reconstruct the same ECEF point.
  NedVelocityView ned_view;
  stat = VelocityViewFrom(loc, ecef_origin, ned_point, &ned_view);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefVelocity reconstructed2;
  stat = VelocityFrom(loc, ecef_origin, ned_view, &reconstructed2);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_DOUBLE_EQ(reconstructed.vx, reconstructed2.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, reconstructed2.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, reconstructed2.vz);

  // Variant dispatch round-trip.
  VelocityTypes v_origin = ecef_origin;
  VelocityTypes v_point  = ned_point;
  EcefVelocityView variant_view;
  stat = VelocityViewFrom(loc, v_origin, v_point, &variant_view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(variant_view.vx, ecef_view.vx);
  ASSERT_DOUBLE_EQ(variant_view.vy, ecef_view.vy);
  ASSERT_DOUBLE_EQ(variant_view.vz, ecef_view.vz);
}

// At (0,0): +Z ECEF = north. origin{0,0,0} + EcefView{0,0,1} = {0,0,1} ECEF =
// {1,0,0} NED.
TEST(VelocityViewExchange, VelocityFromEcefEcefViewNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity point;
  auto stat = VelocityFrom(loc, EcefVelocity{0.0, 0.0, 0.0},
                           EcefVelocityView{0.0, 0.0, 1.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(point.veast, 0.0);
  ASSERT_DOUBLE_EQ(point.vdown, 0.0);
}

// origin{0,0,0} + NedView{1,0,0} north = {0,0,1} ECEF → NED = {1,0,0} north.
TEST(VelocityViewExchange, VelocityFromEcefNedViewNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity point;
  auto stat = VelocityFrom(loc, EcefVelocity{0.0, 0.0, 0.0},
                           NedVelocityView{1.0, 0.0, 0.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(point.veast, 0.0);
  ASSERT_DOUBLE_EQ(point.vdown, 0.0);
}

// NED origin {1,0,0} north = {0,0,1} ECEF. + EcefView{0,0,1} = {0,0,2} ECEF.
TEST(VelocityViewExchange, VelocityFromNedEcefViewEcef) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity point;
  auto stat = VelocityFrom(loc, NedVelocity{1.0, 0.0, 0.0},
                           EcefVelocityView{0.0, 0.0, 1.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, 0.0);
  ASSERT_DOUBLE_EQ(point.vy, 0.0);
  ASSERT_DOUBLE_EQ(point.vz, 2.0);
}

// NED origin {1,0,0} = {0,0,1} ECEF + EcefView{0,0,1} = {0,0,2} ECEF = {2,0,0}
// NED.
TEST(VelocityViewExchange, VelocityFromNedEcefViewNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity point;
  auto stat = VelocityFrom(loc, NedVelocity{1.0, 0.0, 0.0},
                           EcefVelocityView{0.0, 0.0, 1.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vnorth, 2.0);
  ASSERT_DOUBLE_EQ(point.veast, 0.0);
  ASSERT_DOUBLE_EQ(point.vdown, 0.0);
}

// NED origin {1,0,0} + NedView{1,0,0} = {2,0,0} NED = {0,0,2} ECEF.
TEST(VelocityViewExchange, VelocityFromNedNedViewEcef) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity point;
  auto stat = VelocityFrom(loc, NedVelocity{1.0, 0.0, 0.0},
                           NedVelocityView{1.0, 0.0, 0.0}, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, 0.0);
  ASSERT_DOUBLE_EQ(point.vy, 0.0);
  ASSERT_DOUBLE_EQ(point.vz, 2.0);
}

// Two-location VelocityFrom tests.
// When origin_loc == point_loc, results must match the single-location
// overload.

TEST(VelocityViewExchange, TwoLocSameLocMatchesSingleLoc) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity origin{10.0, 20.0, 30.0};
  const NedVelocityView view{1.0, 2.0, 3.0};

  EcefVelocity single_loc_out, two_loc_out;
  ASSERT_EQ(VelocityFrom(loc, origin, view, &single_loc_out), Status::SUCCESS);
  ASSERT_EQ(VelocityFrom(loc, loc, origin, view, &two_loc_out),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(two_loc_out.vx, single_loc_out.vx);
  ASSERT_DOUBLE_EQ(two_loc_out.vy, single_loc_out.vy);
  ASSERT_DOUBLE_EQ(two_loc_out.vz, single_loc_out.vz);

  NedVelocity single_ned_out, two_ned_out;
  ASSERT_EQ(VelocityFrom(loc, origin, view, &single_ned_out), Status::SUCCESS);
  ASSERT_EQ(VelocityFrom(loc, loc, origin, view, &two_ned_out),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(two_ned_out.vnorth, single_ned_out.vnorth);
  ASSERT_DOUBLE_EQ(two_ned_out.veast, single_ned_out.veast);
  ASSERT_DOUBLE_EQ(two_ned_out.vdown, single_ned_out.vdown);
}

// At (0,0): east=+Y ECEF. A NED view of (0,1,0) east at origin_loc=(0,0) is
// (0,1,0) ECEF. At (0,90): +Y ECEF is the outward radial (up), so the same
// ECEF velocity expressed as NED there is (0,0,-1) = 1 m/s up.
TEST(VelocityViewExchange, TwoLocNedViewDifferentOutputLoc) {
  const LlaLocation origin_loc{0.0, 0.0, 0.0};
  const LlaLocation point_loc{0.0, 90.0, 0.0};

  const EcefVelocity origin{0.0, 0.0, 0.0};
  const NedVelocityView view{0.0, 1.0, 0.0};  // 1 m/s east at origin_loc

  // ECEF point = (0,0,0) + NED(0,1,0)@(0,0) = (0,1,0) ECEF.
  // At (0,90): +Y ECEF is outward radial = up, so NED = (0,0,-1).
  NedVelocity out;
  ASSERT_EQ(VelocityFrom(origin_loc, point_loc, origin, view, &out),
            Status::SUCCESS);
  ASSERT_NEAR(out.vnorth, 0.0, 1e-9);
  ASSERT_NEAR(out.veast, 0.0, 1e-9);
  ASSERT_NEAR(out.vdown, -1.0, 1e-9);
}

TEST(VelocityViewExchange, TwoLocVariantDispatch) {
  const LlaLocation origin_loc{0.0, 0.0, 0.0};
  const LlaLocation point_loc{0.0, 90.0, 0.0};
  VelocityTypes origin   = EcefVelocity{0.0, 0.0, 0.0};
  VelocityViewTypes view = NedVelocityView{0.0, 1.0, 0.0};

  NedVelocity out;
  ASSERT_EQ(VelocityFrom(origin_loc, point_loc, origin, view, &out),
            Status::SUCCESS);
  ASSERT_NEAR(out.vnorth, 0.0, 1e-9);
  ASSERT_NEAR(out.veast, 0.0, 1e-9);
  ASSERT_NEAR(out.vdown, -1.0, 1e-9);
}

// AerVelocityView tests.
// Use observer{33.0, 74.0, 0.0} as observer and target{33.5, 74.3, 500.0} as
// target.

static const LlaLocation kObserver{33.0, 74.0, 0.0};
static const LlaLocation kTarget{33.5, 74.3, 500.0};

TEST(VelocityViewExchange, AerVelocityViewFromAerAer) {
  // AerVelocity origin and point: view = point - origin.
  const AerVelocity origin{1.0, 0.5, 100.0};
  const AerVelocity point{3.0, -0.5, 120.0};

  AerVelocityView view;
  auto stat = VelocityViewFrom(kObserver, kTarget, origin, point, &view);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vazimuth, 2.0);
  ASSERT_DOUBLE_EQ(view.velevation, -1.0);
  ASSERT_DOUBLE_EQ(view.vrange, 20.0);

  view = VelocityViewFrom<AerVelocityView>(kObserver, kTarget, origin, point);
  ASSERT_DOUBLE_EQ(view.vazimuth, 2.0);
  ASSERT_DOUBLE_EQ(view.velevation, -1.0);
  ASSERT_DOUBLE_EQ(view.vrange, 20.0);
}

TEST(VelocityViewExchange, AerVelocityViewFromEcefEcef) {
  // Round-trip: VelocityViewFrom then VelocityFrom must reconstruct point.
  const EcefVelocity origin{100.0, -50.0, 25.0};
  const EcefVelocity point{110.0, -48.0, 30.0};

  AerVelocityView view;
  auto stat = VelocityViewFrom(kObserver, kTarget, origin, point, &view);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefVelocity reconstructed;
  stat = VelocityFrom(kObserver, kTarget, origin, view, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(reconstructed.vx, point.vx, 1e-9);
  ASSERT_NEAR(reconstructed.vy, point.vy, 1e-9);
  ASSERT_NEAR(reconstructed.vz, point.vz, 1e-9);
}

TEST(VelocityViewExchange, AerVelocityViewRoundTrip) {
  // Full round-trip with NED origin and NED output.
  const NedVelocity origin{10.0, -5.0, 2.0};
  const NedVelocity point{12.0, -3.0, 1.0};

  AerVelocityView view;
  auto stat = VelocityViewFrom(kObserver, kTarget, origin, point, &view);
  ASSERT_EQ(stat, Status::SUCCESS);

  NedVelocity reconstructed;
  stat = VelocityFrom(kObserver, kTarget, origin, view, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(reconstructed.vnorth, point.vnorth, 1e-9);
  ASSERT_NEAR(reconstructed.veast, point.veast, 1e-9);
  ASSERT_NEAR(reconstructed.vdown, point.vdown, 1e-9);
}

TEST(VelocityViewExchange, AerVelocityViewSingleLocReturnsError) {
  // Single-location calls always return ERROR.
  const LlaLocation loc{33.0, 74.0, 0.0};
  const AerVelocity aer{1.0, 0.0, 10.0};
  const EcefVelocity ecef{1.0, 0.0, 0.0};

  AerVelocityView view_out;
  ASSERT_EQ(VelocityViewFrom(loc, aer, aer, &view_out), Status::ERROR);
  ASSERT_EQ(VelocityViewFrom(loc, ecef, ecef, &view_out), Status::ERROR);

  const AerVelocityView view{1.0, 0.0, 5.0};
  EcefVelocity ecef_out;
  ASSERT_EQ(VelocityFrom(loc, ecef, view, &ecef_out), Status::ERROR);
  AerVelocity aer_out;
  ASSERT_EQ(VelocityFrom(loc, aer, view, &aer_out), Status::ERROR);
}

TEST(VelocityViewExchange, AerVelocityViewVariantDispatch) {
  // Two-location VelocityFrom variant dispatch with AerVelocityView.
  const AerVelocity origin{1.0, 0.5, 100.0};
  const AerVelocity point{3.0, -0.5, 120.0};

  AerVelocityView view;
  VelocityViewFrom(kObserver, kTarget, origin, point, &view);

  VelocityTypes v_origin   = origin;
  VelocityViewTypes v_view = view;
  AerVelocity reconstructed;
  auto stat =
      VelocityFrom(kObserver, kTarget, v_origin, v_view, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(reconstructed.vazimuth, point.vazimuth, 1e-9);
  ASSERT_NEAR(reconstructed.velevation, point.velevation, 1e-9);
  ASSERT_NEAR(reconstructed.vrange, point.vrange, 1e-9);
}

TEST(VelocityViewExchange, AerVelocityViewFarApartNedFrames) {
  // origin_loc and point_loc are 90° longitude apart (~10,000 km).
  // At (0,0): east = (0,1,0) ECEF. At (0,90): east = (-1,0,0) ECEF.
  // The NED frames are rotated 90° relative to each other.
  const LlaLocation origin_loc{0.0, 0.0, 0.0};
  const LlaLocation point_loc{0.0, 90.0, 0.0};

  // origin: 1 m/s north at origin_loc = (0,0,1) ECEF.
  // point:  1 m/s east  at point_loc  = (-1,0,0) ECEF.
  const NedVelocity origin_ned{1.0, 0.0, 0.0};
  const NedVelocity point_ned{0.0, 1.0, 0.0};

  // Compute AerVelocityView from NED inputs in their respective frames.
  AerVelocityView view;
  ASSERT_EQ(VelocityViewFrom(origin_loc, point_loc, origin_ned, point_ned, &view),
            Status::SUCCESS);

  // Reconstruct: VelocityFrom must return NED at point_loc.
  NedVelocity reconstructed;
  ASSERT_EQ(VelocityFrom(origin_loc, point_loc, origin_ned, view, &reconstructed),
            Status::SUCCESS);
  ASSERT_NEAR(reconstructed.vnorth, point_ned.vnorth, 1e-9);
  ASSERT_NEAR(reconstructed.veast, point_ned.veast, 1e-9);
  ASSERT_NEAR(reconstructed.vdown, point_ned.vdown, 1e-9);

  // Cross-check: the same AerVelocityView computed from ECEF equivalents must match.
  // north at (0,0) = (0,0,1) ECEF; east at (0,90) = (-1,0,0) ECEF.
  const EcefVelocity ecef_origin{0.0, 0.0, 1.0};
  const EcefVelocity ecef_point{-1.0, 0.0, 0.0};
  AerVelocityView ecef_view;
  ASSERT_EQ(VelocityViewFrom(origin_loc, point_loc, ecef_origin, ecef_point, &ecef_view),
            Status::SUCCESS);
  ASSERT_NEAR(ecef_view.vazimuth, view.vazimuth, 1e-9);
  ASSERT_NEAR(ecef_view.velevation, view.velevation, 1e-9);
  ASSERT_NEAR(ecef_view.vrange, view.vrange, 1e-9);
}

}  // namespace mutatio
