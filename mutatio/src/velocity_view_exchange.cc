#include "mutatio/velocity_view_exchange.h"

#include "mutatio/velocity_exchange.h"

namespace mutatio {

Status VelocityViewFrom(const LlaLocation&, const EcefVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out) {
  *out = EcefVelocityView{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  return Status::SUCCESS;
}

// The location is accepted for API consistency but not used.
Status VelocityFrom(const LlaLocation&, const EcefVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  *out = EcefVelocity{origin.vx + view.vx, origin.vy + view.vy,
                      origin.vz + view.vz};
  return Status::SUCCESS;
}

// Cross-type EcefVelocityView: convert NED to ECEF, then subtract in ECEF.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, point, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, origin, ecef_point, out);
}

Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, ecef_origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out) {
  EcefVelocity ecef_origin, ecef_point;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  stat = VelocityFrom(loc, point, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, ecef_origin, ecef_point, out);
}

// Cross-type NedVelocityView: convert NED to ECEF, then use the ECEF+ECEF path.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, point, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, origin, ecef_point, out);
}

Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, ecef_origin, point, out);
}

// Computes the ECEF velocity difference and rotates it into the NED frame at
// the given location.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out) {
  const EcefVelocity diff{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  NedVelocity ned_diff;
  auto stat = VelocityFrom(loc, diff, &ned_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = NedVelocityView{ned_diff.vnorth, ned_diff.veast, ned_diff.vdown};
  return Status::SUCCESS;
}

// Both velocities are already in the NED frame, so the view is their
// difference. The location is accepted for API consistency but not used.
Status VelocityViewFrom(const LlaLocation&, const NedVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out) {
  *out =
      NedVelocityView{point.vnorth - origin.vnorth, point.veast - origin.veast,
                      point.vdown - origin.vdown};
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  const EcefVelocity ecef_point{origin.vx + view.vx, origin.vy + view.vy,
                                origin.vz + view.vz};
  return VelocityFrom(loc, ecef_point, out);
}

Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(loc, ecef_point, out);
}

Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{ecef_origin.vx + view.vx, ecef_origin.vy + view.vy,
                      ecef_origin.vz + view.vz};
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(loc, ecef_point, out);
}

Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  NedVelocity ned_point;
  auto stat = VelocityFrom(loc, origin, view, &ned_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(loc, ned_point, out);
}

// Rotates the NED view into the ECEF frame and applies it to the origin.
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  const NedVelocity ned_view{view.vnorth, view.veast, view.vdown};
  EcefVelocity ecef_diff;
  auto stat = VelocityFrom(loc, ned_view, &ecef_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{origin.vx + ecef_diff.vx, origin.vy + ecef_diff.vy,
                      origin.vz + ecef_diff.vz};
  return Status::SUCCESS;
}

// Both origin and view are in the NED frame; the result is their sum.
// The location is accepted for API consistency but not used.
Status VelocityFrom(const LlaLocation&, const NedVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  *out = NedVelocity{origin.vnorth + view.vnorth, origin.veast + view.veast,
                     origin.vdown + view.vdown};
  return Status::SUCCESS;
}

}  // namespace mutatio
