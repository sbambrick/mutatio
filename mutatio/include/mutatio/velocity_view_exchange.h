#pragma once

#include <stdexcept>

#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        EcefVelocityView* out);

Status VelocityFrom(const EcefVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out);

template <class OutViewType, class OriginType, class PointType>
OutViewType VelocityViewFrom(const OriginType& origin, const PointType& point) {
  OutViewType out;
  auto status = VelocityViewFrom(origin, point, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityViewFrom.");
  }

  return out;
}

template <class OutVelType, class OriginType, class ViewType>
OutVelType VelocityFrom(const OriginType& origin, const ViewType& view) {
  OutVelType out;
  auto status = VelocityFrom(origin, view, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityFrom.");
  }

  return out;
}

template <class ViewType, class = std::enable_if_t<is_one_of_variants_types<
                              VelocityViewTypes, ViewType>>>
Status VelocityViewFrom(const VelocityTypes& origin, const VelocityTypes& point,
                        ViewType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &point, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, out](const auto& pt) {
                      status = VelocityViewFrom(orig, pt, out);
                    },
                },
                point);
          },
      },
      origin);
  return status;
}

template <class VelType, class = std::enable_if_t<
                             is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const VelocityTypes& origin,
                    const VelocityViewTypes& view_variant, VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_variant, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, out](const auto& view) {
                      status = VelocityFrom(orig, view, out);
                    },
                },
                view_variant);
          },
      },
      origin);
  return status;
}

}  // namespace mutatio
