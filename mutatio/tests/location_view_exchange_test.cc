#include "mutatio/location_view_exchange.h"

#include <cmath>

#include "gtest/gtest.h"

#include "mutatio/location.h"
#include "mutatio/location_view.h"
#include "mutatio/status.h"

namespace mutatio {

// --- ViewFrom(origin, point, View*) : construct a view from two locations ---

TEST(LocationViewExchange, ViewFromTwoLocationsToNed) {
  const LlaLocation origin{0.0, 0.0, 0.0};

  // A point ~1000 m due north should have north ≈ 1000, east ≈ 0, down = 0.
  const LlaLocation north_point{0.009, 0.0, 0.0};
  NedLocationView result;
  auto stat = ViewFrom(origin, north_point, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 1000.0, 10.0);
  ASSERT_NEAR(result.east, 0.0, 1e-6);
  ASSERT_DOUBLE_EQ(result.down, 0.0);

  // Same location → zero view.
  stat = ViewFrom(origin, origin, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.north, 0.0);
  ASSERT_DOUBLE_EQ(result.east, 0.0);
  ASSERT_DOUBLE_EQ(result.down, 0.0);

  // Point 1000 m directly above → down = -1000.
  const LlaLocation above{0.0, 0.0, 1000.0};
  stat = ViewFrom(origin, above, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 0.0, 1e-6);
  ASSERT_NEAR(result.east, 0.0, 1e-6);
  ASSERT_DOUBLE_EQ(result.down, -1000.0);
}

TEST(LocationViewExchange, ViewFromTwoLocationsToAer) {
  const LlaLocation origin{0.0, 0.0, 0.0};

  // ~1000 m due north: azimuth = 0°, elevation ≈ 0°.
  const LlaLocation north_point{0.009, 0.0, 0.0};
  AerLocationView result;
  auto stat = ViewFrom(origin, north_point, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.azimuth, 0.0, 1e-9);
  ASSERT_NEAR(result.elevation, 0.0, 1e-6);
  ASSERT_NEAR(result.range, 1000.0, 10.0);

  // ~1000 m due east: azimuth = 90°.
  const LlaLocation east_point{0.0, 0.009, 0.0};
  stat = ViewFrom(origin, east_point, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.azimuth, 90.0, 1e-6);
  ASSERT_NEAR(result.elevation, 0.0, 1e-6);

  // 1000 m directly above: elevation = 90°, range = 1000 m.
  const LlaLocation above{0.0, 0.0, 1000.0};
  stat = ViewFrom(origin, above, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.elevation, 90.0, 1e-6);
  ASSERT_DOUBLE_EQ(result.range, 1000.0);
}

TEST(LocationViewExchange, ViewFromTwoLocationsToEcef) {
  // At (0°N, 0°E, 0m), ECEF = (6378137, 0, 0).
  // A point 100 m above has ECEF = (6378237, 0, 0), so the view is (100, 0, 0).
  const LlaLocation origin{0.0, 0.0, 0.0};
  const LlaLocation above{0.0, 0.0, 100.0};

  EcefLocationView result;
  auto stat = ViewFrom(origin, above, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.x, 100.0, 1e-6);
  ASSERT_NEAR(result.y, 0.0, 1e-6);
  ASSERT_NEAR(result.z, 0.0, 1e-6);

  // Same location → zero view.
  stat = ViewFrom(origin, origin, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.x, 0.0);
  ASSERT_DOUBLE_EQ(result.y, 0.0);
  ASSERT_DOUBLE_EQ(result.z, 0.0);
}

// ViewFrom accepts EcefLocation as origin type.
TEST(LocationViewExchange, ViewFromEcefOrigin) {
  const EcefLocation origin_ecef{6378137.0, 0.0, 0.0};  // (0°N, 0°E, 0m)
  const LlaLocation north_point{0.009, 0.0, 0.0};

  NedLocationView result;
  auto stat = ViewFrom(origin_ecef, north_point, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 1000.0, 10.0);
  ASSERT_NEAR(result.east, 0.0, 1e-6);
}

// --- ViewFrom throwing template wrapper ---

TEST(LocationViewExchange, ViewFromThrowingWrapper) {
  const LlaLocation origin{0.0, 0.0, 0.0};
  const LlaLocation north_point{0.009, 0.0, 0.0};

  auto ned = ViewFrom<NedLocationView>(origin, north_point);
  ASSERT_NEAR(ned.north, 1000.0, 10.0);
  ASSERT_NEAR(ned.east, 0.0, 1e-6);

  auto aer = ViewFrom<AerLocationView>(origin, north_point);
  ASSERT_NEAR(aer.azimuth, 0.0, 1e-9);
  ASSERT_GT(aer.range, 0.0);

  auto ecef = ViewFrom<EcefLocationView>(origin, north_point);
  ASSERT_NEAR(ecef.x, 0.0, 1.0);
  ASSERT_NEAR(ecef.y, 0.0, 1e-6);
  ASSERT_NEAR(ecef.z, 1000.0, 10.0);
}

// --- ViewFrom identity: same input and output view type ---

TEST(LocationViewExchange, ViewFromNedToNedIdentity) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const NedLocationView ned{100.0, 200.0, -50.0};

  NedLocationView result;
  auto stat = ViewFrom(origin, ned, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.north, ned.north);
  ASSERT_DOUBLE_EQ(result.east, ned.east);
  ASSERT_DOUBLE_EQ(result.down, ned.down);
}

TEST(LocationViewExchange, ViewFromAerToAerIdentity) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const AerLocationView aer{45.0, 30.0, 500.0};

  AerLocationView result;
  auto stat = ViewFrom(origin, aer, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.azimuth, aer.azimuth);
  ASSERT_DOUBLE_EQ(result.elevation, aer.elevation);
  ASSERT_DOUBLE_EQ(result.range, aer.range);
}

TEST(LocationViewExchange, ViewFromEcefToEcefIdentity) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const EcefLocationView ecef{100.0, 200.0, 300.0};

  EcefLocationView result;
  auto stat = ViewFrom(origin, ecef, &result);

  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.x, ecef.x);
  ASSERT_DOUBLE_EQ(result.y, ecef.y);
  ASSERT_DOUBLE_EQ(result.z, ecef.z);
}

// --- ViewFrom cross-type conversions: AER <-> NED ---

TEST(LocationViewExchange, ViewFromAerToNed) {
  const LlaLocation origin{0.0, 0.0, 0.0};

  // AER due north, flat → NED (north=1000, east=0, down=0).
  NedLocationView result;
  auto stat = ViewFrom(origin, AerLocationView{0.0, 0.0, 1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 1000.0, 1e-9);
  ASSERT_NEAR(result.east, 0.0, 1e-9);
  ASSERT_NEAR(result.down, 0.0, 1e-9);

  // AER due east → NED (north=0, east=1000, down=0).
  stat = ViewFrom(origin, AerLocationView{90.0, 0.0, 1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 0.0, 1e-9);
  ASSERT_NEAR(result.east, 1000.0, 1e-9);
  ASSERT_NEAR(result.down, 0.0, 1e-9);

  // AER straight up (elevation=90°) → NED (north=0, east=0, down=-1000).
  stat = ViewFrom(origin, AerLocationView{0.0, 90.0, 1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 0.0, 1e-9);
  ASSERT_NEAR(result.east, 0.0, 1e-9);
  ASSERT_NEAR(result.down, -1000.0, 1e-9);
}

TEST(LocationViewExchange, ViewFromNedToAer) {
  const LlaLocation origin{0.0, 0.0, 0.0};

  // NED due north (north=1000) → azimuth=0°, elevation=0°, range=1000m.
  AerLocationView result;
  auto stat = ViewFrom(origin, NedLocationView{1000.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.azimuth, 0.0, 1e-9);
  ASSERT_NEAR(result.elevation, 0.0, 1e-9);
  ASSERT_NEAR(result.range, 1000.0, 1e-9);

  // NED due east (east=1000) → azimuth=90°.
  stat = ViewFrom(origin, NedLocationView{0.0, 1000.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.azimuth, 90.0, 1e-9);
  ASSERT_NEAR(result.elevation, 0.0, 1e-9);
  ASSERT_NEAR(result.range, 1000.0, 1e-9);

  // NED straight up (down=-1000) → elevation=90°, range=1000m.
  stat = ViewFrom(origin, NedLocationView{0.0, 0.0, -1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.elevation, 90.0, 1e-9);
  ASSERT_NEAR(result.range, 1000.0, 1e-9);
}

// AER→NED→AER should reconstruct the original AER view.
TEST(LocationViewExchange, ViewFromAerNedRoundTrip) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const AerLocationView aer_in{45.0, 30.0, 500.0};

  NedLocationView ned;
  auto stat = ViewFrom(origin, aer_in, &ned);
  ASSERT_EQ(stat, Status::SUCCESS);

  AerLocationView aer_out;
  stat = ViewFrom(origin, ned, &aer_out);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(aer_out.azimuth, aer_in.azimuth, 1e-9);
  ASSERT_NEAR(aer_out.elevation, aer_in.elevation, 1e-9);
  ASSERT_NEAR(aer_out.range, aer_in.range, 1e-9);
}

// --- ViewFrom cross-type conversions: NED/AER <-> ECEF ---

TEST(LocationViewExchange, ViewFromNedToEcef) {
  // At (0°N, 0°E, 0m): north = +Z ECEF, east = +Y ECEF, up = +X ECEF.
  const LlaLocation origin{0.0, 0.0, 0.0};
  EcefLocationView result;

  // 1000m north → +Z ECEF ≈ 1000. A northward displacement produces a tiny
  // negative x due to Earth's curvature; use a loose tolerance for x.
  auto stat = ViewFrom(origin, NedLocationView{1000.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.x, 0.0, 1.0);
  ASSERT_NEAR(result.y, 0.0, 1e-6);
  ASSERT_NEAR(result.z, 1000.0, 10.0);

  // 1000m east → +Y ECEF ≈ 1000.
  stat = ViewFrom(origin, NedLocationView{0.0, 1000.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.x, 0.0, 1.0);
  ASSERT_NEAR(result.y, 1000.0, 10.0);
  ASSERT_NEAR(result.z, 0.0, 1e-6);

  // 1000m up (down=-1000) → +X ECEF ≈ 1000.
  stat = ViewFrom(origin, NedLocationView{0.0, 0.0, -1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.x, 1000.0, 1e-6);
  ASSERT_NEAR(result.y, 0.0, 1e-6);
  ASSERT_NEAR(result.z, 0.0, 1e-6);
}

TEST(LocationViewExchange, ViewFromAerToEcef) {
  // AER→ECEF round-trips with ECEF→AER.
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const AerLocationView aer{45.0, 30.0, 500.0};

  EcefLocationView ecef_result;
  auto stat = ViewFrom(origin, aer, &ecef_result);
  ASSERT_EQ(stat, Status::SUCCESS);

  AerLocationView aer_reconstructed;
  stat = ViewFrom(origin, ecef_result, &aer_reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(aer_reconstructed.azimuth, aer.azimuth, 1e-9);
  ASSERT_NEAR(aer_reconstructed.elevation, aer.elevation, 1e-9);
  ASSERT_NEAR(aer_reconstructed.range, aer.range, 1e-9);
}

TEST(LocationViewExchange, ViewFromEcefToNed) {
  // At (0°N, 0°E, 0m): +Z ECEF = north, +Y ECEF = east.
  const LlaLocation origin{0.0, 0.0, 0.0};
  NedLocationView result;

  // +Z ECEF view → north ≈ 1000, east ≈ 0. The point is not exactly on the
  // ellipsoid surface so there is a small non-zero down component (~0.08 m).
  auto stat = ViewFrom(origin, EcefLocationView{0.0, 0.0, 1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 1000.0, 10.0);
  ASSERT_NEAR(result.east, 0.0, 1e-6);
  ASSERT_NEAR(result.down, 0.0, 1.0);

  // +Y ECEF view → east ≈ 1000, north ≈ 0.
  stat = ViewFrom(origin, EcefLocationView{0.0, 1000.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.north, 0.0, 1e-6);
  ASSERT_NEAR(result.east, 1000.0, 10.0);
  ASSERT_NEAR(result.down, 0.0, 1.0);
}

TEST(LocationViewExchange, ViewFromEcefToAer) {
  // ECEF→AER→ECEF should round-trip.
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const EcefLocationView ecef_view{100.0, 200.0, 300.0};

  AerLocationView aer_result;
  auto stat = ViewFrom(origin, ecef_view, &aer_result);
  ASSERT_EQ(stat, Status::SUCCESS);

  EcefLocationView ecef_reconstructed;
  stat = ViewFrom(origin, aer_result, &ecef_reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(ecef_reconstructed.x, ecef_view.x, 1e-6);
  ASSERT_NEAR(ecef_reconstructed.y, ecef_view.y, 1e-6);
  ASSERT_NEAR(ecef_reconstructed.z, ecef_view.z, 1e-6);
}

// --- LocationFrom(origin, view, point*) ---

TEST(LocationViewExchange, LocationFromOriginPlusNed) {
  const LlaLocation origin{0.0, 0.0, 0.0};
  LlaLocation result;

  // Zero view → same location.
  auto stat = LocationFrom(origin, NedLocationView{0.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_DOUBLE_EQ(result.lat, 0.0);
  ASSERT_DOUBLE_EQ(result.lon, 0.0);
  ASSERT_DOUBLE_EQ(result.alt, 0.0);

  // 1000m north → lat increases, lon stays 0.
  stat = LocationFrom(origin, NedLocationView{1000.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_GT(result.lat, 0.0);
  ASSERT_NEAR(result.lon, 0.0, 1e-9);
  ASSERT_NEAR(result.alt, 0.0, 1e-6);

  // 1000m up (down=-1000) → altitude = 1000m, position unchanged.
  stat = LocationFrom(origin, NedLocationView{0.0, 0.0, -1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.lat, 0.0, 1e-9);
  ASSERT_NEAR(result.lon, 0.0, 1e-9);
  ASSERT_NEAR(result.alt, 1000.0, 1e-6);
}

TEST(LocationViewExchange, LocationFromOriginPlusAer) {
  const LlaLocation origin{0.0, 0.0, 0.0};
  LlaLocation result;

  // Zero range → same location.
  auto stat = LocationFrom(origin, AerLocationView{0.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.lat, 0.0, 1e-9);
  ASSERT_NEAR(result.lon, 0.0, 1e-9);
  ASSERT_NEAR(result.alt, 0.0, 1e-6);

  // Straight up (elevation=90°, range=1000m) → altitude = 1000m.
  stat = LocationFrom(origin, AerLocationView{0.0, 90.0, 1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.lat, 0.0, 1e-6);
  ASSERT_NEAR(result.lon, 0.0, 1e-6);
  ASSERT_NEAR(result.alt, 1000.0, 1e-6);

  // Due north (az=0°, el=0°, range=1000m) → lat increases.
  stat = LocationFrom(origin, AerLocationView{0.0, 0.0, 1000.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_GT(result.lat, 0.0);
  ASSERT_NEAR(result.lon, 0.0, 1e-9);
}

TEST(LocationViewExchange, LocationFromOriginPlusEcef) {
  // At (0°N, 0°E, 0m), ECEF = (6378137, 0, 0). Moving +100 in X → alt ≈ 100.
  const LlaLocation origin{0.0, 0.0, 0.0};
  LlaLocation result;
  auto stat = LocationFrom(origin, EcefLocationView{100.0, 0.0, 0.0}, &result);
  ASSERT_EQ(stat, Status::SUCCESS);
  ASSERT_NEAR(result.lat, 0.0, 1e-9);
  ASSERT_NEAR(result.lon, 0.0, 1e-9);
  ASSERT_NEAR(result.alt, 100.0, 1e-6);
}

// LocationFrom throwing template wrapper.
TEST(LocationViewExchange, LocationFromThrowingWrapper) {
  const LlaLocation origin{0.0, 0.0, 0.0};

  auto result =
      LocationFrom<LlaLocation>(origin, AerLocationView{0.0, 0.0, 1000.0});
  ASSERT_GT(result.lat, 0.0);
  ASSERT_NEAR(result.lon, 0.0, 1e-9);

  EcefLocation ecef_result =
      LocationFrom<EcefLocation>(origin, EcefLocationView{100.0, 0.0, 0.0});
  ASSERT_NEAR(ecef_result.x, 6378237.0, 1e-6);
  ASSERT_NEAR(ecef_result.y, 0.0, 1e-6);
  ASSERT_NEAR(ecef_result.z, 0.0, 1e-6);
}

// --- Round-trip: ViewFrom two locs → LocationFrom → reconstructed point ---

TEST(LocationViewExchange, NedRoundTrip) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 550.0};

  NedLocationView ned;
  auto stat = ViewFrom(origin, point, &ned);
  ASSERT_EQ(stat, Status::SUCCESS);

  LlaLocation reconstructed;
  stat = LocationFrom(origin, ned, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(reconstructed.lat, point.lat, 1e-9);
  ASSERT_NEAR(reconstructed.lon, point.lon, 1e-9);
  ASSERT_NEAR(reconstructed.alt, point.alt, 1e-6);
}

TEST(LocationViewExchange, AerRoundTrip) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 550.0};

  AerLocationView aer;
  auto stat = ViewFrom(origin, point, &aer);
  ASSERT_EQ(stat, Status::SUCCESS);

  LlaLocation reconstructed;
  stat = LocationFrom(origin, aer, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(reconstructed.lat, point.lat, 1e-9);
  ASSERT_NEAR(reconstructed.lon, point.lon, 1e-9);
  ASSERT_NEAR(reconstructed.alt, point.alt, 1e-6);
}

TEST(LocationViewExchange, EcefViewRoundTrip) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 550.0};

  EcefLocationView ecef_view;
  auto stat = ViewFrom(origin, point, &ecef_view);
  ASSERT_EQ(stat, Status::SUCCESS);

  LlaLocation reconstructed;
  stat = LocationFrom(origin, ecef_view, &reconstructed);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(reconstructed.lat, point.lat, 1e-9);
  ASSERT_NEAR(reconstructed.lon, point.lon, 1e-9);
  ASSERT_NEAR(reconstructed.alt, point.alt, 1e-6);
}

// --- Variant dispatch ---

TEST(LocationViewExchange, LocationFromVariantViewDispatch) {
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 550.0};

  // Build the view and hold it in a LocationViewTypes variant.
  AerLocationView aer_direct;
  ViewFrom(origin, point, &aer_direct);
  const LocationViewTypes aer_variant = aer_direct;

  LlaLocation result_from_variant;
  auto stat = LocationFrom(origin, aer_variant, &result_from_variant);
  ASSERT_EQ(stat, Status::SUCCESS);

  // Must match the concrete-type path.
  LlaLocation result_direct;
  LocationFrom(origin, aer_direct, &result_direct);
  ASSERT_DOUBLE_EQ(result_from_variant.lat, result_direct.lat);
  ASSERT_DOUBLE_EQ(result_from_variant.lon, result_direct.lon);
  ASSERT_DOUBLE_EQ(result_from_variant.alt, result_direct.alt);
}

TEST(LocationViewExchange, LocationFromVariantOriginAndViewDispatch) {
  const LocationTypes origin_variant = LlaLocation{33.0, 74.0, 1000.0};
  const LlaLocation origin{33.0, 74.0, 1000.0};
  const LlaLocation point{33.5, 74.3, 550.0};

  AerLocationView aer;
  ViewFrom(origin, point, &aer);
  const LocationViewTypes view_variant = aer;

  LlaLocation result;
  auto stat = LocationFrom(origin_variant, view_variant, &result);
  ASSERT_EQ(stat, Status::SUCCESS);

  ASSERT_NEAR(result.lat, point.lat, 1e-9);
  ASSERT_NEAR(result.lon, point.lon, 1e-9);
  ASSERT_NEAR(result.alt, point.alt, 1e-6);
}

}  // namespace mutatio
