#include "mutatio/location_view_exchange.h"

#include <cmath>

#include "GeographicLib/Geodesic.hpp"

namespace mutatio::internal {

Status ViewFromLla(const LlaLocation& origin, const LlaLocation& point,
                   NedLocationView* out) {
  const GeographicLib::Geodesic earth(kWgs84A + origin.alt, kWgs84F);

  Meter s12;
  Degree azi1, azi2;
  earth.Inverse(origin.lat, origin.lon, point.lat, point.lon, s12, azi1, azi2);

  const auto north = s12 * std::cos(azi1 * kDegToRad);
  const auto east  = s12 * std::sin(azi1 * kDegToRad);
  const auto down  = origin.alt - point.alt;

  *out = NedLocationView{north, east, down};
  return Status::SUCCESS;
}

Status ViewFromLla(const LlaLocation& origin, const LlaLocation& point,
                   AerLocationView* out) {
  const GeographicLib::Geodesic earth(kWgs84A + origin.alt, kWgs84F);

  Meter s12;
  Degree azi1, azi2;
  earth.Inverse(origin.lat, origin.lon, point.lat, point.lon, s12, azi1, azi2);

  const Meter h12  = point.alt - origin.alt;
  const auto elev  = std::atan2(h12, s12) / kDegToRad;
  const auto range = std::sqrt((s12 * s12) + (h12 * h12));

  *out = AerLocationView{azi1, elev, range};
  return Status::SUCCESS;
}

Status PointLlaFromNed(const LlaLocation& origin, const NedLocationView& ned,
                       LlaLocation* out) {
  const GeographicLib::Geodesic earth(kWgs84A + origin.alt, kWgs84F);

  const Meter s12 = std::sqrt((ned.north * ned.north) + (ned.east * ned.east));
  const Degree azi1 = std::atan2(ned.east, ned.north) / kDegToRad;

  Degree lat2, lon2, azi2;
  earth.Direct(origin.lat, origin.lon, azi1, s12, lat2, lon2, azi2);

  *out = LlaLocation{lat2, lon2, origin.alt - ned.down};
  return Status::SUCCESS;
}

Status PointLlaFromAer(const LlaLocation& origin, const AerLocationView& aer,
                       LlaLocation* out) {
  const GeographicLib::Geodesic earth(kWgs84A + origin.alt, kWgs84F);

  const Meter s12 = aer.range * std::cos(aer.elevation * kDegToRad);

  Degree lat2, lon2, azi2;
  earth.Direct(origin.lat, origin.lon, aer.azimuth, s12, lat2, lon2, azi2);

  const auto h = aer.range * std::sin(aer.elevation * kDegToRad);
  *out         = LlaLocation{lat2, lon2, origin.alt + h};
  return Status::SUCCESS;
}

}  // namespace mutatio::internal
