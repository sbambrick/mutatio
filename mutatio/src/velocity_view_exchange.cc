#include "mutatio/velocity_view_exchange.h"

#include "mutatio/velocity_exchange.h"

namespace mutatio {

Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        EcefVelocityView* out) {
  *out = EcefVelocityView{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  return Status::SUCCESS;
}

Status VelocityFrom(const EcefVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out) {
  *out = EcefVelocity{origin.vx + view.vx, origin.vy + view.vy,
                      origin.vz + view.vz};
  return Status::SUCCESS;
}

// Computes the ECEF velocity difference and rotates it into the NED frame at
// the given location.
Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        const LlaLocation& loc, NedVelocityView* out) {
  const EcefVelocity diff{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  NedVelocity ned_diff;
  auto stat = VelocityFrom(diff, loc, &ned_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = NedVelocityView{ned_diff.vnorth, ned_diff.veast, ned_diff.vdown};
  return Status::SUCCESS;
}

// Both velocities are already in the NED frame, so the view is their
// difference. The location is accepted for API consistency but not used.
Status VelocityViewFrom(const NedVelocity& origin, const NedVelocity& point,
                        const LlaLocation&, NedVelocityView* out) {
  *out = NedVelocityView{point.vnorth - origin.vnorth,
                         point.veast - origin.veast,
                         point.vdown - origin.vdown};
  return Status::SUCCESS;
}

// Rotates the NED view into the ECEF frame and applies it to the origin.
Status VelocityFrom(const EcefVelocity& origin, const NedVelocityView& view,
                    const LlaLocation& loc, EcefVelocity* out) {
  const NedVelocity ned_view{view.vnorth, view.veast, view.vdown};
  EcefVelocity ecef_diff;
  auto stat = VelocityFrom(ned_view, loc, &ecef_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{origin.vx + ecef_diff.vx, origin.vy + ecef_diff.vy,
                      origin.vz + ecef_diff.vz};
  return Status::SUCCESS;
}

// Both origin and view are in the NED frame; the result is their sum.
// The location is accepted for API consistency but not used.
Status VelocityFrom(const NedVelocity& origin, const NedVelocityView& view,
                    const LlaLocation&, NedVelocity* out) {
  *out = NedVelocity{origin.vnorth + view.vnorth, origin.veast + view.veast,
                     origin.vdown + view.vdown};
  return Status::SUCCESS;
}

}  // namespace mutatio
