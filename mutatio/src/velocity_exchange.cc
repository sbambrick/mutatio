#include "mutatio/velocity_exchange.h"

#include <cmath>
#include <limits>

#include "GeographicLib/Constants.hpp"

#include "mutatio/location_view_exchange.h"

namespace mutatio {

namespace {

// Computes the AER-to-NED Jacobian. Given the AER position and AerVelocity
// (angular rates in deg/s, range rate in m/s), returns the equivalent
// NedVelocity in the NED frame at origin_loc.
Status AerToNedAtOrigin(const AerLocationView& pos, const AerVelocity& in,
                        NedVelocity* out) {
  const auto deg    = GeographicLib::Constants::degree();
  const auto vel_az = in.vazimuth * deg;    // deg/s → rad/s
  const auto vel_el = in.velevation * deg;  // deg/s → rad/s

  const auto cos_az = std::cos(pos.azimuth * deg);
  const auto sin_az = std::sin(pos.azimuth * deg);
  const auto cos_el = std::cos(pos.elevation * deg);
  const auto sin_el = std::sin(pos.elevation * deg);

  *out = NedVelocity{
      cos_el * cos_az * in.vrange - pos.range * sin_el * cos_az * vel_el -
          pos.range * cos_el * sin_az * vel_az,
      cos_el * sin_az * in.vrange - pos.range * sin_el * sin_az * vel_el +
          pos.range * cos_el * cos_az * vel_az,
      -sin_el * in.vrange - pos.range * cos_el * vel_el,
  };
  return Status::SUCCESS;
}

// Computes the NED-to-AER inverse Jacobian. NedVelocity is expressed in the
// NED frame at origin_loc. Returns Status::ERROR on singularity (r ≈ 0 or
// elevation ≈ ±90°).
Status NedAtOriginToAer(const AerLocationView& pos, const NedVelocity& in,
                        AerVelocity* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto cos_az  = std::cos(pos.azimuth * deg);
  const auto sin_az  = std::sin(pos.azimuth * deg);
  const auto cos_el  = std::cos(pos.elevation * deg);
  const auto sin_el  = std::sin(pos.elevation * deg);
  const auto epsilon = std::numeric_limits<double>::epsilon() * 1e6;

  if (pos.range < epsilon) return Status::ERROR;
  if (std::abs(cos_el) < epsilon) return Status::ERROR;

  const auto vrange = cos_el * cos_az * in.vnorth + cos_el * sin_az * in.veast -
                      sin_el * in.vdown;
  const auto vel_el_rad = (-sin_el * cos_az * in.vnorth -
                           sin_el * sin_az * in.veast - cos_el * in.vdown) /
                          pos.range;
  const auto vel_az_rad =
      (-sin_az * in.vnorth + cos_az * in.veast) / (pos.range * cos_el);

  *out = AerVelocity{vel_az_rad / deg, vel_el_rad / deg, vrange};
  return Status::SUCCESS;
}

}  // namespace

// Rotates an ECEF velocity into the NED frame at the given location using the
// standard ECEF-to-NED rotation matrix:
//   [ -sin(lat)cos(lon)  -sin(lat)sin(lon)   cos(lat) ]
//   [ -sin(lon)           cos(lon)            0        ]
//   [ -cos(lat)cos(lon)  -cos(lat)sin(lon)  -sin(lat)  ]
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& in,
                    NedVelocity* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto sin_lat = std::sin(loc.lat * deg);
  const auto cos_lat = std::cos(loc.lat * deg);
  const auto sin_lon = std::sin(loc.lon * deg);
  const auto cos_lon = std::cos(loc.lon * deg);

  *out = NedVelocity{
      -sin_lat * cos_lon * in.vx - sin_lat * sin_lon * in.vy + cos_lat * in.vz,
      -sin_lon * in.vx + cos_lon * in.vy,
      -cos_lat * cos_lon * in.vx - cos_lat * sin_lon * in.vy - sin_lat * in.vz,
  };
  return Status::SUCCESS;
}

// Rotates a NED velocity back into the ECEF frame using the transpose of the
// ECEF-to-NED rotation matrix.
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& in,
                    EcefVelocity* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto sin_lat = std::sin(loc.lat * deg);
  const auto cos_lat = std::cos(loc.lat * deg);
  const auto sin_lon = std::sin(loc.lon * deg);
  const auto cos_lon = std::cos(loc.lon * deg);

  *out = EcefVelocity{
      -sin_lat * cos_lon * in.vnorth - sin_lon * in.veast -
          cos_lat * cos_lon * in.vdown,
      -sin_lat * sin_lon * in.vnorth + cos_lon * in.veast -
          cos_lat * sin_lon * in.vdown,
      cos_lat * in.vnorth - sin_lat * in.vdown,
  };
  return Status::SUCCESS;
}

// The location is accepted for API consistency but not used.
Status VelocityFrom(const LlaLocation&, const EcefVelocity& in,
                    EcefVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation&, const NedVelocity& in,
                    NedVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

// Two-location VelocityFrom — origin_loc defines the NED frame of the input;
// point_loc defines the NED frame of the output. All arithmetic is in ECEF.

// Both locations unused — ECEF velocity is frame-independent.
Status VelocityFrom(const LlaLocation&, const LlaLocation&,
                    const EcefVelocity& in, EcefVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

// Rotate ECEF to NED at point_loc; origin_loc unused.
Status VelocityFrom(const LlaLocation&, const LlaLocation& point_loc,
                    const EcefVelocity& in, NedVelocity* out) {
  return VelocityFrom(point_loc, in, out);
}

// Rotate NED to ECEF at origin_loc; point_loc unused.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation&,
                    const NedVelocity& in, EcefVelocity* out) {
  return VelocityFrom(origin_loc, in, out);
}

// Rotate NED to ECEF at origin_loc, then ECEF to NED at point_loc.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& in, NedVelocity* out) {
  EcefVelocity ecef;
  auto stat = VelocityFrom(origin_loc, in, &ecef);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(point_loc, ecef, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& in, EcefVelocity* out) {
  AerLocationView aer_pos;
  auto stat = ViewFrom(origin_loc, point_loc, &aer_pos);
  if (stat != Status::SUCCESS) return stat;

  NedVelocity ned_at_origin;
  stat = AerToNedAtOrigin(aer_pos, in, &ned_at_origin);
  if (stat != Status::SUCCESS) return stat;

  // NED at origin_loc → ECEF.
  return VelocityFrom(origin_loc, ned_at_origin, out);
}

// Output NedVelocity is in the NED frame at origin_loc (the observer's frame).
// This is the natural output for AER conversions since the AER frame is
// defined at origin_loc.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& in, NedVelocity* out) {
  AerLocationView aer_pos;
  auto stat = ViewFrom(origin_loc, point_loc, &aer_pos);
  if (stat != Status::SUCCESS) return stat;

  return AerToNedAtOrigin(aer_pos, in, out);
}

// The locations are accepted for API consistency but not used.
Status VelocityFrom(const LlaLocation&, const LlaLocation&,
                    const AerVelocity& in, AerVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& in, AerVelocity* out) {
  AerLocationView aer_pos;
  auto stat = ViewFrom(origin_loc, point_loc, &aer_pos);
  if (stat != Status::SUCCESS) return stat;

  // ECEF → NED at origin_loc.
  NedVelocity ned_at_origin;
  stat = VelocityFrom(origin_loc, in, &ned_at_origin);
  if (stat != Status::SUCCESS) return stat;

  return NedAtOriginToAer(aer_pos, ned_at_origin, out);
}

// Input NedVelocity is in NED at origin_loc (per two-location convention).
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& in, AerVelocity* out) {
  AerLocationView aer_pos;
  auto stat = ViewFrom(origin_loc, point_loc, &aer_pos);
  if (stat != Status::SUCCESS) return stat;

  return NedAtOriginToAer(aer_pos, in, out);
}

}  // namespace mutatio
