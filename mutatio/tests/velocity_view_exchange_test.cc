#include "mutatio/velocity_view_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/location_view.h"
#include "mutatio/location_view_exchange.h"
#include "mutatio/status.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

// Two-location VelocityViewFrom and VelocityFrom tests.

TEST(VelocityViewExchange, TwoLocEcefVelocityView) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocity point{15.0, 18.0, 35.0};
  EcefVelocityView expected{5.0, -2.0, 5.0};

  EcefVelocityView view;
  ASSERT_EQ(VelocityViewFrom(loc, loc, origin, point, &view), Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);
}

TEST(VelocityViewExchange, TwoLocEcefEcefViewEcef) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  EcefVelocity result;
  ASSERT_EQ(VelocityFrom(loc, loc, EcefVelocity{10.0, 20.0, 30.0},
                         EcefVelocityView{5.0, -2.0, 5.0}, &result),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.vx, 15.0);
  ASSERT_DOUBLE_EQ(result.vy, 18.0);
  ASSERT_DOUBLE_EQ(result.vz, 35.0);
}

TEST(VelocityViewExchange, TwoLocEcefRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocity point{15.0, 18.0, 35.0};

  auto view          = VelocityViewFrom<EcefVelocityView>(loc, loc, origin, point);
  auto reconstructed = VelocityFrom<EcefVelocity>(loc, loc, origin, view);

  ASSERT_DOUBLE_EQ(reconstructed.vx, point.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, point.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, point.vz);
}

// At (0 N, 0 E): +Z ECEF = north, so a diff of (0,0,1) in ECEF -> (1,0,0) NED.
TEST(VelocityViewExchange, TwoLocNedVelocityView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocityView view;
  ASSERT_EQ(VelocityViewFrom(loc, loc, EcefVelocity{0.0, 0.0, 1.0},
                             EcefVelocity{0.0, 0.0, 2.0}, &view),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);
}

// NED north at (0,0) = +Z ECEF, so origin{0,0,0} + view{1,0,0} = {0,0,1}.
TEST(VelocityViewExchange, TwoLocVelocityFromNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity point;
  ASSERT_EQ(VelocityFrom(loc, loc, EcefVelocity{0.0, 0.0, 0.0},
                         NedVelocityView{1.0, 0.0, 0.0}, &point),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, 0.0);
  ASSERT_DOUBLE_EQ(point.vy, 0.0);
  ASSERT_DOUBLE_EQ(point.vz, 1.0);
}

TEST(VelocityViewExchange, TwoLocNedRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity origin{10.0, 20.0, 30.0};
  const EcefVelocity point{15.0, 18.0, 35.0};

  auto view          = VelocityViewFrom<NedVelocityView>(loc, loc, origin, point);
  auto reconstructed = VelocityFrom<EcefVelocity>(loc, loc, origin, view);

  ASSERT_DOUBLE_EQ(reconstructed.vx, point.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, point.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, point.vz);
}

TEST(VelocityViewExchange, TwoLocVariantsRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  VelocityTypes origin = EcefVelocity{10.0, 20.0, 30.0};
  VelocityTypes point  = EcefVelocity{15.0, 18.0, 35.0};

  NedVelocityView view;
  ASSERT_EQ(VelocityViewFrom(loc, loc, origin, point, &view), Status::SUCCESS);

  EcefVelocity reconstructed;
  ASSERT_EQ(
      VelocityFrom(loc, loc, origin, VelocityViewTypes{view}, &reconstructed),
      Status::SUCCESS);
  ASSERT_DOUBLE_EQ(reconstructed.vx, std::get<EcefVelocity>(point).vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, std::get<EcefVelocity>(point).vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, std::get<EcefVelocity>(point).vz);
}

// AerVelocityView tests.
// Use observer{33.0, 74.0, 0.0} as observer and target{33.5, 74.3, 500.0}.

static const LlaLocation kObserver{33.0, 74.0, 0.0};
static const LlaLocation kTarget{33.5, 74.3, 500.0};

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

// Two-location throwing wrapper — AerVelocityView with singular geometry.
TEST(VelocityViewExchange, ThrowingWrappers) {
  // Two-loc VelocityViewFrom throw: singular geometry (origin == point ->
  // range=0 -> inverse Jacobian undefined).
  const LlaLocation loc{33.0, 74.0, 0.0};
  EXPECT_THROW(
      (VelocityViewFrom<AerVelocityView>(loc, loc, EcefVelocity{1.0, 0.0, 0.0},
                                         EcefVelocity{2.0, 0.0, 0.0})),
      std::invalid_argument);
}

// LocType template overloads — only instantiated with non-LlaLocation types.
TEST(VelocityViewExchange, EcefLocTwoLocTemplates) {
  const LlaLocation lla_obs{33.0, 74.0, 0.0};
  const LlaLocation lla_tgt{33.5, 74.3, 500.0};
  const EcefLocation ecef_obs = LocationFrom<EcefLocation>(lla_obs);
  const EcefLocation ecef_tgt = LocationFrom<EcefLocation>(lla_tgt);

  EcefVelocity ecef_out;
  const LlaLocation lla_loc{0.0, 0.0, 0.0};
  const EcefLocation ecef_loc = LocationFrom<EcefLocation>(lla_loc);

  EcefVelocityView ecef_view;
  NedVelocityView ned_view;

  ASSERT_EQ(VelocityViewFrom(ecef_loc, ecef_loc, EcefVelocity{0, 0, 0},
                             EcefVelocity{0, 0, 1}, &ecef_view),
            Status::SUCCESS);
  ASSERT_EQ(VelocityViewFrom(ecef_loc, ecef_loc, EcefVelocity{0, 0, 0},
                             EcefVelocity{0, 0, 1}, &ned_view),
            Status::SUCCESS);
  ASSERT_EQ(VelocityFrom(ecef_loc, ecef_loc, EcefVelocity{0, 0, 0},
                         EcefVelocityView{0, 0, 1}, &ecef_out),
            Status::SUCCESS);
  ASSERT_EQ(VelocityFrom(ecef_loc, ecef_loc, EcefVelocity{0, 0, 0},
                         NedVelocityView{1, 0, 0}, &ecef_out),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(ecef_out.vz, 1.0);

  // Two-loc AerVelocityView VelocityViewFrom and VelocityFrom templates.
  AerVelocityView aer_view;
  ASSERT_EQ(VelocityViewFrom(ecef_obs, ecef_tgt, EcefVelocity{100.0, -50.0, 25.0},
                             EcefVelocity{110.0, -48.0, 30.0}, &aer_view),
            Status::SUCCESS);
  ASSERT_EQ(VelocityFrom(ecef_obs, ecef_tgt, EcefVelocity{100.0, -50.0, 25.0},
                         aer_view, &ecef_out),
            Status::SUCCESS);
}

// LocationView overload tests.
// Each test derives a LocationView from two known locations and verifies the
// result matches the direct two-location call.

TEST(VelocityViewExchange, LocViewEcefVelocityView) {
  const AerLocationView aer_loc =
      ViewFrom<AerLocationView>(kObserver, kTarget);

  const EcefVelocity origin{100.0, -50.0, 25.0};
  const EcefVelocity point{110.0, -48.0, 30.0};

  EcefVelocityView expected;
  ASSERT_EQ(VelocityViewFrom(kObserver, kTarget, origin, point, &expected),
            Status::SUCCESS);

  EcefVelocityView view;
  ASSERT_EQ(VelocityViewFrom(kObserver, aer_loc, origin, point, &view),
            Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);
}

TEST(VelocityViewExchange, LocViewNedVelocityView) {
  const AerLocationView aer_loc =
      ViewFrom<AerLocationView>(kObserver, kTarget);

  const EcefVelocity origin{100.0, -50.0, 25.0};
  const EcefVelocity point{110.0, -48.0, 30.0};

  NedVelocityView expected;
  ASSERT_EQ(VelocityViewFrom(kObserver, kTarget, origin, point, &expected),
            Status::SUCCESS);

  NedVelocityView view;
  ASSERT_EQ(VelocityViewFrom(kObserver, aer_loc, origin, point, &view),
            Status::SUCCESS);
  ASSERT_NEAR(view.vnorth, expected.vnorth, 1e-9);
  ASSERT_NEAR(view.veast, expected.veast, 1e-9);
  ASSERT_NEAR(view.vdown, expected.vdown, 1e-9);
}

TEST(VelocityViewExchange, LocViewAerVelocityViewRoundTrip) {
  // Derive AerLocationView from two known locations, use it to compute an
  // AerVelocityView, then reconstruct the point velocity — must match the
  // direct two-location call.
  const AerLocationView aer_loc =
      ViewFrom<AerLocationView>(kObserver, kTarget);

  const EcefVelocity origin{100.0, -50.0, 25.0};
  const EcefVelocity point{110.0, -48.0, 30.0};

  AerVelocityView view;
  ASSERT_EQ(VelocityViewFrom(kObserver, aer_loc, origin, point, &view),
            Status::SUCCESS);

  EcefVelocity reconstructed;
  ASSERT_EQ(VelocityFrom(kObserver, aer_loc, origin, view, &reconstructed),
            Status::SUCCESS);
  ASSERT_NEAR(reconstructed.vx, point.vx, 1e-9);
  ASSERT_NEAR(reconstructed.vy, point.vy, 1e-9);
  ASSERT_NEAR(reconstructed.vz, point.vz, 1e-9);
}

TEST(VelocityViewExchange, LocViewMatchesTwoLoc) {
  // Verify that passing an AerLocationView derived from kObserver+kTarget
  // produces the same AerVelocityView as the direct two-location call.
  const AerLocationView aer_loc =
      ViewFrom<AerLocationView>(kObserver, kTarget);

  const EcefVelocity origin{100.0, -50.0, 25.0};
  const EcefVelocity point{110.0, -48.0, 30.0};

  AerVelocityView two_loc_view, loc_view_view;
  ASSERT_EQ(
      VelocityViewFrom(kObserver, kTarget, origin, point, &two_loc_view),
      Status::SUCCESS);
  ASSERT_EQ(VelocityViewFrom(kObserver, aer_loc, origin, point, &loc_view_view),
            Status::SUCCESS);

  ASSERT_NEAR(loc_view_view.vazimuth, two_loc_view.vazimuth, 1e-9);
  ASSERT_NEAR(loc_view_view.velevation, two_loc_view.velevation, 1e-9);
  ASSERT_NEAR(loc_view_view.vrange, two_loc_view.vrange, 1e-9);
}

}  // namespace mutatio
