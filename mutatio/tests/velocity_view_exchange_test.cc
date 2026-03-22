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

// At (0,0): +Z ECEF = north. origin{0,0,0} + EcefView{0,0,1} = {0,0,1} ECEF = {1,0,0} NED.
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

// NED origin {1,0,0} = {0,0,1} ECEF + EcefView{0,0,1} = {0,0,2} ECEF = {2,0,0} NED.
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

}  // namespace mutatio
