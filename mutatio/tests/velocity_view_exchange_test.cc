#include "mutatio/velocity_view_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/status.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

TEST(VelocityViewExchange, VelocityViewFromEcefEcef) {
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocity point{15.0, 18.0, 35.0};
  EcefVelocityView expected{5.0, -2.0, 5.0};

  EcefVelocityView view;
  auto stat = VelocityViewFrom(origin, point, &view);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);

  view = VelocityViewFrom<EcefVelocityView>(origin, point);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);
}

TEST(VelocityViewExchange, VelocityViewFromVariants) {
  VelocityTypes origin = EcefVelocity{10.0, 20.0, 30.0};
  VelocityTypes point  = EcefVelocity{15.0, 18.0, 35.0};
  EcefVelocityView expected{5.0, -2.0, 5.0};

  EcefVelocityView view;
  auto stat = VelocityViewFrom(origin, point, &view);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vx, expected.vx);
  ASSERT_DOUBLE_EQ(view.vy, expected.vy);
  ASSERT_DOUBLE_EQ(view.vz, expected.vz);
}

TEST(VelocityViewExchange, VelocityFromEcefView) {
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocityView view{5.0, -2.0, 5.0};
  EcefVelocity expected{15.0, 18.0, 35.0};

  EcefVelocity point;
  auto stat = VelocityFrom(origin, view, &point);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, expected.vx);
  ASSERT_DOUBLE_EQ(point.vy, expected.vy);
  ASSERT_DOUBLE_EQ(point.vz, expected.vz);

  point = VelocityFrom<EcefVelocity>(origin, view);
  ASSERT_DOUBLE_EQ(point.vx, expected.vx);
  ASSERT_DOUBLE_EQ(point.vy, expected.vy);
  ASSERT_DOUBLE_EQ(point.vz, expected.vz);
}

TEST(VelocityViewExchange, VelocityFromVariants) {
  VelocityTypes origin           = EcefVelocity{10.0, 20.0, 30.0};
  VelocityViewTypes view_variant = EcefVelocityView{5.0, -2.0, 5.0};
  EcefVelocity expected{15.0, 18.0, 35.0};

  EcefVelocity point;
  auto stat = VelocityFrom(origin, view_variant, &point);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, expected.vx);
  ASSERT_DOUBLE_EQ(point.vy, expected.vy);
  ASSERT_DOUBLE_EQ(point.vz, expected.vz);
}

TEST(VelocityViewExchange, RoundTrip) {
  EcefVelocity origin{10.0, 20.0, 30.0};
  EcefVelocity point{15.0, 18.0, 35.0};

  auto view          = VelocityViewFrom<EcefVelocityView>(origin, point);
  auto reconstructed = VelocityFrom<EcefVelocity>(origin, view);

  ASSERT_DOUBLE_EQ(reconstructed.vx, point.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, point.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, point.vz);
}

// At (0 N, 0 E): +Z ECEF = north, so a diff of (0,0,1) in ECEF -> (1,0,0) NED.
TEST(VelocityViewExchange, VelocityViewFromEcefNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocityView view;
  Status stat;

  stat = VelocityViewFrom(EcefVelocity{0.0, 0.0, 1.0},
                          EcefVelocity{0.0, 0.0, 2.0}, loc, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);

  view = VelocityViewFrom<NedVelocityView>(EcefVelocity{0.0, 0.0, 1.0},
                                           EcefVelocity{0.0, 0.0, 2.0}, loc);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 0.0);
  ASSERT_DOUBLE_EQ(view.vdown, 0.0);
}

TEST(VelocityViewExchange, VelocityViewFromNedNed) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocityView view;

  auto stat = VelocityViewFrom(NedVelocity{10.0, 20.0, 30.0},
                               NedVelocity{11.0, 22.0, 33.0}, loc, &view);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(view.vnorth, 1.0);
  ASSERT_DOUBLE_EQ(view.veast, 2.0);
  ASSERT_DOUBLE_EQ(view.vdown, 3.0);
}

TEST(VelocityViewExchange, VelocityFromEcefNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  EcefVelocity point;

  // NED north at (0,0) = +Z ECEF, so origin{0,0,0} + view{1,0,0} = {0,0,1}.
  auto stat =
      VelocityFrom(EcefVelocity{0.0, 0.0, 0.0}, NedVelocityView{1.0, 0.0, 0.0},
                   loc, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vx, 0.0);
  ASSERT_DOUBLE_EQ(point.vy, 0.0);
  ASSERT_DOUBLE_EQ(point.vz, 1.0);

  point = VelocityFrom<EcefVelocity>(EcefVelocity{0.0, 0.0, 0.0},
                                     NedVelocityView{1.0, 0.0, 0.0}, loc);
  ASSERT_DOUBLE_EQ(point.vz, 1.0);
}

TEST(VelocityViewExchange, VelocityFromNedNedView) {
  const LlaLocation loc{0.0, 0.0, 0.0};
  NedVelocity point;

  auto stat = VelocityFrom(NedVelocity{10.0, 20.0, 30.0},
                           NedVelocityView{1.0, 2.0, 3.0}, loc, &point);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(point.vnorth, 11.0);
  ASSERT_DOUBLE_EQ(point.veast, 22.0);
  ASSERT_DOUBLE_EQ(point.vdown, 33.0);
}

TEST(VelocityViewExchange, NedRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  const EcefVelocity origin{10.0, 20.0, 30.0};
  const EcefVelocity point{15.0, 18.0, 35.0};

  auto view          = VelocityViewFrom<NedVelocityView>(origin, point, loc);
  auto reconstructed = VelocityFrom<EcefVelocity>(origin, view, loc);

  ASSERT_DOUBLE_EQ(reconstructed.vx, point.vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, point.vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, point.vz);
}

TEST(VelocityViewExchange, NedVariantsRoundTrip) {
  const LlaLocation loc{33.0, 74.0, 1000.0};
  VelocityTypes origin = EcefVelocity{10.0, 20.0, 30.0};
  VelocityTypes point  = EcefVelocity{15.0, 18.0, 35.0};

  NedVelocityView view;
  auto stat = VelocityViewFrom(origin, point, loc, &view);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefVelocity reconstructed;
  stat = VelocityFrom(origin, VelocityViewTypes{view}, loc, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(reconstructed.vx, std::get<EcefVelocity>(point).vx);
  ASSERT_DOUBLE_EQ(reconstructed.vy, std::get<EcefVelocity>(point).vy);
  ASSERT_DOUBLE_EQ(reconstructed.vz, std::get<EcefVelocity>(point).vz);
}

}  // namespace mutatio
