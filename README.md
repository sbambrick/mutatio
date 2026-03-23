# Mutatio

Mutatio is a type-safe wrapper around geographic conversions where the desired
conversion to/from absolute and relative coordinate systems is enforced by the
C++ type system.

```c++
// Throws when the conversion fails...
auto loc = LocationFrom<EcefLocation>(other_loc);

// Relative coordinate conversions...
auto view = ViewFrom<AerLocationView>(origin_loc, point_loc);
```

## Concepts

### Location

Locations represent fixed points on the Earth.

* LlaLocation - A coordinate system where Latitude, Longitude, and Altitude are
  the degrees above/below the equator, the degrees east/west from the meridian,
  and the meters above/below the WGS84 ellipsoid. Also known as the geodetic
  coordinate system.
* EcefLocation - A coordinate system where X, Y, and Z are the distance in
  meters from the center of the Earth. Also known as the geocentric coordinate
  system.

```c++
#include "mutatio/location.h"
#include "mutatio/location_exchange.h"

...

// Define a fixed Location.
LlaLocation lla_loc{33.0, 74.0, 1000.0};

// Convert to another Location coordinate system.
auto ecef_loc = LocationFrom<EcefLocation>(lla_loc);

// Convert to a pre-allocated Location.
EcefLocation pre_alloc_loc;
auto stat = LocationFrom(lla_loc, &pre_alloc_loc);

// Convert an arbitrary Location into a specific Location type.
LocationTypes any_loc = EcefLocation{200.0, 200.0, 200.0};
auto specific_loc     = LocationFrom<LlaLocation>(any_loc);
```

### LocationView

LocationViews represent relative distances between an origin Location and
another Location with respect to a given coordinate frame.

* NedLocationView - A coordinate system representing the North, East, and Down
  distance in meters from an origin Location to another Location. North and East
  are defined with respect to the Latitude and Longitude at that origin Location
  while Down is defined with respect to the change in WGS84 altitude at that
  Location.
* AerLocationView - A coordinate system representing the Azimuth, Elevation,
  and Range in degrees and meters from an origin Location to another Location.
  Azimuth is defined clockwise from North, and Elevation is defined with respect
  to the WGS84 ellipsoid.
* EcefLocationView - A coordinate system representing the distance in meters
  from an origin Location to another Location with respect to the ECEF
  coordinate system.

```c++
#include "mutatio/location_view.h"
#include "mutatio/location_view_exchange.h"

...

// Define a View from a Location.
NedLocationView ned_view{10.0, 15.0, 10.0};

// Construct a View of another Location from an origin Location.
LlaLocation origin_loc{33.0, 74.0, 1000.0};
LlaLocation point_loc{33.5, 74.3, 550.0};
auto aer_view = ViewFrom<AerLocationView>(origin_loc, point_loc);

// Construct a pre-allocated View.
NedLocationView pre_alloc_view;
auto stat = ViewFrom(origin_loc, point_loc, &pre_alloc_view);

// Convert one View type into another View type.
auto ecef_view = ViewFrom<EcefLocationView>(origin_loc, ned_view);

// Construct a Location from an origin Location and a View.
auto new_loc = LocationFrom<EcefLocation>(origin_loc, aer_view);

// Construct a Location from an origin Location and an abitrary View.
LocationViewTypes any_view = NedLocationView{1.0, 2.0, 3.0};
auto specific_loc = LocationFrom<LlaLocation>(origin_loc, any_view);

// ... or even both an arbitrary origin Location and View.
LocationTypes any_loc = EcefLocation{200.0, 200.0, 200.0};
specific_loc          = LocationFrom<LlaLocation>(any_loc, any_view);
```

### Velocity

Velocities mirror Locations and are absolute. Converting between coordinate
frames requires a reference Location to define the local orientation of the
WGS84 ellipsoid.

* EcefVelocity - A velocity in meters per second expressed in the ECEF
  coordinate frame.
* NedVelocity - A velocity in meters per second expressed as North, East, and
  Down components relative to the WGS84 ellipsoid at a given Location.
* AerVelocity - A velocity expressed as the time derivatives of Azimuth
  (deg/s), Elevation (deg/s), and Range (m/s) as observed from an origin
  Location. Because the Jacobian depends on the instantaneous geometry,
  AerVelocity conversions require two locations: `origin_loc` (the observer)
  and `point_loc` (the target's current position). Single-location calls
  return `Status::ERROR`. The NedVelocity output from an AerVelocity
  conversion is expressed in the NED frame at `origin_loc`.

```c++
#include "mutatio/velocity.h"
#include "mutatio/velocity_exchange.h"

...

// Define a velocity in the ECEF frame.
EcefVelocity ecef_vel{100.0, -50.0, 25.0};

// Convert to a NED velocity at a given location.
LlaLocation loc{33.0, 74.0, 1000.0};
auto ned_vel = VelocityFrom<NedVelocity>(loc, ecef_vel);

// Convert to a pre-allocated Velocity.
NedVelocity pre_alloc_vel;
auto stat = VelocityFrom(loc, ecef_vel, &pre_alloc_vel);

// Convert back to ECEF.
auto ecef_vel_out = VelocityFrom<EcefVelocity>(loc, ned_vel);

// Convert arbitrary Velocity and Location variants.
VelocityTypes any_vel = EcefVelocity{100.0, -50.0, 25.0};
LocationTypes any_loc = LlaLocation{33.0, 74.0, 1000.0};
NedVelocity specific_vel;
stat = VelocityFrom(any_loc, any_vel, &specific_vel);

// Two-location conversion — origin_loc defines the NED frame of the input
// velocity and point_loc defines the NED frame of the output. All arithmetic
// is done in ECEF to avoid composing rotation matrices.
LlaLocation origin_loc{0.0, 0.0, 0.0};
LlaLocation point_loc{0.0, 90.0, 0.0};
NedVelocity ned_at_origin{0.0, 1.0, 0.0};  // east at origin_loc

// Express the same velocity in the NED frame at point_loc.
auto ned_at_point = VelocityFrom<NedVelocity>(origin_loc, point_loc, ned_at_origin);

// Pre-allocated and variant forms are also supported.
NedVelocity pre_alloc;
stat = VelocityFrom(origin_loc, point_loc, ned_at_origin, &pre_alloc);
stat = VelocityFrom(any_loc, any_loc, any_vel, &specific_vel);
```

AerVelocity conversions always use the two-location form. The NedVelocity
output is in the NED frame at `origin_loc`.

```c++
#include "mutatio/velocity.h"
#include "mutatio/velocity_exchange.h"

...

LlaLocation observer{33.0, 74.0, 0.0}; // observer
LlaLocation target{33.5, 74.3, 500.0}; // target's current position

// Convert a measured AerVelocity to NedVelocity (in the observers's NED frame).
AerVelocity aer_vel{0.5, -0.2, 120.0};  // deg/s, deg/s, m/s
auto ned_vel = VelocityFrom<NedVelocity>(observer, target, aer_vel);

// Convert NedVelocity (at observer) back to AerVelocity.
auto reconstructed = VelocityFrom<AerVelocity>(observer, target, ned_vel);

// Convert to ECEF velocity.
auto ecef_vel = VelocityFrom<EcefVelocity>(observer, target, aer_vel);

// Pre-allocated form.
AerVelocity aer_out;
stat = VelocityFrom(observer, target, ned_vel, &aer_out);

// AerVelocity can be held in VelocityTypes; two-location variant dispatch works.
VelocityTypes vel_variant = aer_vel;
NedVelocity ned_out;
stat = VelocityFrom(observer, target, vel_variant, &ned_out);
```

### VelocityView

VelocityViews represent the relative velocity between an origin Velocity and
another Velocity with respect to a given coordinate frame.

* EcefVelocityView - The difference in meters per second between two velocities
  expressed in the ECEF coordinate frame.
* NedVelocityView - The difference in meters per second between two velocities
  expressed as North, East, and Down components.
* AerVelocityView - The difference between two velocities expressed as Azimuth
  rate (deg/s), Elevation rate (deg/s), and Range rate (m/s) in the AER frame
  defined by `origin_loc` (observer) and `point_loc` (target position).
  AerVelocityView conversions always use the two-location form — single-location
  calls return `Status::ERROR`.

`EcefVelocityView` and `NedVelocityView` conversions require a single reference
Location. `AerVelocityView` conversions require two locations (`origin_loc` and
`point_loc`) to define the AER geometry. Cross-type conversions are fully
supported: origin, view, and output can each be in different frames.

For `NedVelocity` inputs and outputs, `AerVelocityView` follows the same
two-location frame convention as `VelocityFrom`: `origin_loc` defines the NED
frame of the origin velocity, `point_loc` defines the NED frame of the point
velocity and any `NedVelocity` output.

```c++
#include "mutatio/velocity_view.h"
#include "mutatio/velocity_view_exchange.h"

...

LlaLocation loc{33.0, 74.0, 1000.0};
EcefVelocity origin_vel{10.0, 20.0, 30.0};
EcefVelocity point_vel{15.0, 18.0, 35.0};

// Construct an EcefVelocityView between two velocities.
auto ecef_vel_view = VelocityViewFrom<EcefVelocityView>(loc, origin_vel, point_vel);

// Construct a pre-allocated EcefVelocityView.
EcefVelocityView pre_alloc_view;
auto stat = VelocityViewFrom(loc, origin_vel, point_vel, &pre_alloc_view);

// Reconstruct a Velocity from an origin Velocity and a VelocityView.
// The output type is independent of the origin and view frame.
auto point_ecef = VelocityFrom<EcefVelocity>(loc, origin_vel, ecef_vel_view);
auto point_ned  = VelocityFrom<NedVelocity>(loc, origin_vel, ecef_vel_view);

// Construct a NedVelocityView between two velocities.
auto ned_vel_view = VelocityViewFrom<NedVelocityView>(loc, origin_vel, point_vel);

// Reconstruct from a NedVelocityView — output frame is independent.
auto point_ecef2 = VelocityFrom<EcefVelocity>(loc, origin_vel, ned_vel_view);
auto point_ned2  = VelocityFrom<NedVelocity>(loc, origin_vel, ned_vel_view);

// Cross-type: origin, view, and output can all be in different frames.
NedVelocity ned_origin{10.0, 20.0, 30.0};
auto cross_ecef_view = VelocityViewFrom<EcefVelocityView>(loc, ned_origin, point_vel);
auto cross_ned_view  = VelocityViewFrom<NedVelocityView>(loc, ned_origin, point_vel);
auto cross_output    = VelocityFrom<NedVelocity>(loc, ned_origin, cross_ecef_view);

// Variant dispatch requires a location for all VelocityViewFrom and VelocityFrom calls.
VelocityTypes any_origin = EcefVelocity{10.0, 20.0, 30.0};
VelocityTypes any_point  = NedVelocity{1.0, 2.0, 3.0};
EcefVelocityView variant_view;
stat = VelocityViewFrom(loc, any_origin, any_point, &variant_view);

// ... or using arbitrary Velocity and VelocityView variants.
// The output type is independent of the variant frames held at runtime.
VelocityViewTypes any_vel_view = EcefVelocityView{5.0, -2.0, 5.0};
EcefVelocity ecef_out;
stat = VelocityFrom(loc, any_origin, any_vel_view, &ecef_out);
NedVelocity ned_out;
stat = VelocityFrom(loc, any_origin, any_vel_view, &ned_out);

// AerVelocityView — always requires two locations.
LlaLocation observer{33.0, 74.0, 0.0};
LlaLocation target{33.5, 74.3, 500.0};

// origin_vel is in the NED frame at observer; point_vel is in the NED frame at target.
NedVelocity origin_vel{10.0, 5.0, 0.0};
NedVelocity point_vel{12.0, 3.0, -1.0};
AerVelocityView aer_vel_view;
stat = VelocityViewFrom(observer, target, origin_vel, point_vel, &aer_vel_view);

// Reconstruct point_vel — output NedVelocity is in the NED frame at target.
NedVelocity reconstructed;
stat = VelocityFrom(observer, target, origin_vel, aer_vel_view, &reconstructed);

// Origin and point velocities can be in any frame; output frame is independent.
EcefVelocity ecef_origin_vel{100.0, -50.0, 25.0};
auto aer_view2 = VelocityViewFrom<AerVelocityView>(observer, target,
                                                   ecef_origin_vel, point_vel);
auto point_ecef = VelocityFrom<EcefVelocity>(observer, target,
                                             ecef_origin_vel, aer_view2);
```

## Quick Start

**CMake + Conan:**
```bash
conan build . --build=missing
cmake --build build/Release --target coverage
cmake --build build/Release --target format
```

**Bazel:**
```bash
bazel test //mutatio:all_tests                       # run all tests
bazel test //mutatio:location_exchange_test          # run one test
bazel run //:format                                  # format sources
bazel run //:check_format                            # check formatting
```
All dependencies are fetched automatically from the [Bazel Central Registry](https://registry.bazel.build). Requires Bazel 9 (see `.bazelversion`).
