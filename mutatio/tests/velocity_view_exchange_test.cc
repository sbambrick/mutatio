#include "mutatio/velocity_view_exchange.h"

#include "gtest/gtest.h"

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

}  // namespace mutatio
