#include "mutatio/location_exchange.h"

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/status.h"

namespace mutatio {

TEST(LocationExchange, LocationFromLlaLla) {
  // LocationFrom should return a copy of the original.
  LlaLocation original{1.0, 2.0, 3.0};
  LlaLocation copy;
  auto stat = LocationFrom(original, &copy);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(original.lat, copy.lat);
  ASSERT_DOUBLE_EQ(original.lon, copy.lon);
  ASSERT_DOUBLE_EQ(original.alt, copy.alt);

  LocationTypes variant = original;
  stat                  = LocationFrom(variant, &copy);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(original.lat, copy.lat);
  ASSERT_DOUBLE_EQ(original.lon, copy.lon);
  ASSERT_DOUBLE_EQ(original.alt, copy.alt);

  copy = LocationFrom<LlaLocation>(original);
  ASSERT_DOUBLE_EQ(original.lat, copy.lat);
  ASSERT_DOUBLE_EQ(original.lon, copy.lon);
  ASSERT_DOUBLE_EQ(original.alt, copy.alt);
}

TEST(LocationExchange, LocationFromLlaEcef) {
  LlaLocation lla_loc;
  EcefLocation ecef_loc, expected;
  Status stat;

  // 0, 0, 0 LLA <-> Earth radius at X
  lla_loc  = LlaLocation{0.0, 0.0, 0.0};
  stat     = LocationFrom(lla_loc, &ecef_loc);
  expected = EcefLocation{6378137, 0.0, 0.0};
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(ecef_loc.x, expected.x);
  ASSERT_DOUBLE_EQ(ecef_loc.y, expected.y);
  ASSERT_DOUBLE_EQ(ecef_loc.z, expected.z);

  // 0, 90, 0 LLA <-> Earth radius at Y
  lla_loc  = LlaLocation{0.0, 90.0, 0.0};
  stat     = LocationFrom(lla_loc, &ecef_loc);
  expected = EcefLocation{0.0, 6378137, 0.0};
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(ecef_loc.x, expected.x);
  ASSERT_DOUBLE_EQ(ecef_loc.y, expected.y);
  ASSERT_DOUBLE_EQ(ecef_loc.z, expected.z);

  LocationTypes variant = lla_loc;
  stat                  = LocationFrom(variant, &ecef_loc);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(ecef_loc.x, expected.x);
  ASSERT_DOUBLE_EQ(ecef_loc.y, expected.y);
  ASSERT_DOUBLE_EQ(ecef_loc.z, expected.z);

  ecef_loc = LocationFrom<EcefLocation>(lla_loc);
  ASSERT_DOUBLE_EQ(ecef_loc.x, expected.x);
  ASSERT_DOUBLE_EQ(ecef_loc.y, expected.y);
  ASSERT_DOUBLE_EQ(ecef_loc.z, expected.z);
}

TEST(LocationExchange, LocationFromEcefLla) {
  EcefLocation ecef_loc;
  LlaLocation lla_loc, expected;
  Status stat;

  // 0, 0, 0 LLA <-> Earth radius at X
  ecef_loc = EcefLocation{6378137, 0.0, 0.0};
  stat     = LocationFrom(ecef_loc, &lla_loc);
  expected = LlaLocation{0.0, 0.0, 0.0};
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(lla_loc.lat, expected.lat);
  ASSERT_DOUBLE_EQ(lla_loc.lon, expected.lon);
  ASSERT_DOUBLE_EQ(lla_loc.alt, expected.alt);

  // 0, 90, 0 LLA <-> Earth radius at Y
  ecef_loc = EcefLocation{0.0, 6378137, 0.0};
  stat     = LocationFrom(ecef_loc, &lla_loc);
  expected = LlaLocation{0.0, 90.0, 0.0};
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(lla_loc.lat, expected.lat);
  ASSERT_DOUBLE_EQ(lla_loc.lon, expected.lon);
  ASSERT_DOUBLE_EQ(lla_loc.alt, expected.alt);

  LocationTypes variant = ecef_loc;
  stat                  = LocationFrom(variant, &lla_loc);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(lla_loc.lat, expected.lat);
  ASSERT_DOUBLE_EQ(lla_loc.lon, expected.lon);
  ASSERT_DOUBLE_EQ(lla_loc.alt, expected.alt);

  lla_loc = LocationFrom<LlaLocation>(ecef_loc);
  ASSERT_DOUBLE_EQ(lla_loc.lat, expected.lat);
  ASSERT_DOUBLE_EQ(lla_loc.lon, expected.lon);
  ASSERT_DOUBLE_EQ(lla_loc.alt, expected.alt);
}

TEST(LocationExchange, LocationFromEcefEcef) {
  // LocationFrom should return a copy of the original.
  EcefLocation original{1.0, 2.0, 3.0};
  EcefLocation copy;
  auto stat = LocationFrom(original, &copy);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(original.x, copy.x);
  ASSERT_DOUBLE_EQ(original.y, copy.y);
  ASSERT_DOUBLE_EQ(original.z, copy.z);

  LocationTypes variant = original;
  stat                  = LocationFrom(variant, &copy);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(original.x, copy.x);
  ASSERT_DOUBLE_EQ(original.y, copy.y);
  ASSERT_DOUBLE_EQ(original.z, copy.z);

  copy = LocationFrom<EcefLocation>(original);

  ASSERT_DOUBLE_EQ(original.x, copy.x);
  ASSERT_DOUBLE_EQ(original.y, copy.y);
  ASSERT_DOUBLE_EQ(original.z, copy.z);
}

TEST(LocationExchange, LocationFromAssignment) {
  // LocationFrom should return a copy of the original.
  EcefLocation original{1.0, 2.0, 3.0};
  LocationTypes variant = original;
  auto copy             = LocationFrom<EcefLocation>(variant);

  ASSERT_DOUBLE_EQ(original.x, copy.x);
  ASSERT_DOUBLE_EQ(original.y, copy.y);
  ASSERT_DOUBLE_EQ(original.z, copy.z);
}

}  // namespace mutatio
