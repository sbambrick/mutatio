#include "mutatio/location_exchange.h"

#include "GeographicLib/Geocentric.hpp"

namespace mutatio {

Status LocationFrom(const LlaLocation &in, LlaLocation *out) {
  *out = in;
  return Status::SUCCESS;
}

Status LocationFrom(const LlaLocation &in, EcefLocation *out) {
  const GeographicLib::Geocentric earth(GeographicLib::Constants::WGS84_a(),
                                        GeographicLib::Constants::WGS84_f());
  earth.Forward(in.lat, in.lon, in.alt, out->x, out->y, out->z);
  return Status::SUCCESS;
}

Status LocationFrom(const EcefLocation &in, LlaLocation *out) {
  const GeographicLib::Geocentric earth(GeographicLib::Constants::WGS84_a(),
                                        GeographicLib::Constants::WGS84_f());
  earth.Reverse(in.x, in.y, in.z, out->lat, out->lon, out->alt);
  return Status::SUCCESS;
}

Status LocationFrom(const EcefLocation &in, EcefLocation *out) {
  *out = in;
  return Status::SUCCESS;
}

}  // namespace mutatio
