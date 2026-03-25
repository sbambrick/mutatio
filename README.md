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

Velocities are absolute and expressed in the ECEF frame. Because ECEF is
frame-independent, no reference Location is needed for velocity conversions.

* EcefVelocity - A velocity in meters per second expressed in the ECEF
  coordinate frame.

```c++
#include "mutatio/velocity.h"
#include "mutatio/velocity_exchange.h"

...

// Define a velocity in the ECEF frame.
EcefVelocity ecef_vel{100.0, -50.0, 25.0};

// Copy to a pre-allocated Velocity (identity; no location needed).
EcefVelocity out;
auto stat = VelocityFrom(ecef_vel, &out);

// Throwing form.
auto ecef_copy = VelocityFrom<EcefVelocity>(ecef_vel);

// Variant dispatch — also requires no location.
VelocityTypes any_vel = EcefVelocity{100.0, -50.0, 25.0};
EcefVelocity specific_vel;
stat = VelocityFrom(any_vel, &specific_vel);
```

### VelocityView

VelocityViews represent the relative velocity between an origin Velocity and
another Velocity with respect to a given coordinate frame.

* EcefVelocityView - The difference in meters per second between two velocities
  expressed in the ECEF coordinate frame.
* NedVelocityView - The difference in meters per second between two velocities
  expressed as North, East, and Down components. Uses `origin_loc` to define
  the NED frame orientation.
* AerVelocityView - The difference between two velocities expressed as Azimuth
  rate (deg/s), Elevation rate (deg/s), and Range rate (m/s). Uses both
  `origin_loc` and `point_loc` to define the AER geometry.

The input velocity is always `EcefVelocity`. All VelocityView conversions
require an origin location plus either a point location or a `LocationView`
encoding the relative position of the point.

```c++
#include "mutatio/velocity_view.h"
#include "mutatio/velocity_view_exchange.h"

...

LlaLocation origin_loc{33.0, 74.0, 1000.0};
LlaLocation point_loc{33.5, 74.3, 500.0};
EcefVelocity origin_vel{10.0, 20.0, 30.0};
EcefVelocity point_vel{15.0, 18.0, 35.0};

// Construct an EcefVelocityView between two velocities.
auto ecef_vel_view = VelocityViewFrom<EcefVelocityView>(origin_loc, point_loc,
                                                        origin_vel, point_vel);

// Construct a pre-allocated EcefVelocityView.
EcefVelocityView pre_alloc_view;
auto stat = VelocityViewFrom(origin_loc, point_loc, origin_vel, point_vel,
                             &pre_alloc_view);

// Reconstruct a Velocity from an origin Velocity and a VelocityView.
auto point_ecef = VelocityFrom<EcefVelocity>(origin_loc, point_loc, origin_vel,
                                             ecef_vel_view);

// Construct a NedVelocityView.
auto ned_vel_view = VelocityViewFrom<NedVelocityView>(origin_loc, point_loc,
                                                      origin_vel, point_vel);

// Reconstruct from a NedVelocityView.
auto point_ecef2 = VelocityFrom<EcefVelocity>(origin_loc, point_loc, origin_vel,
                                              ned_vel_view);

// AerVelocityView.
AerVelocityView aer_vel_view;
stat = VelocityViewFrom(origin_loc, point_loc, origin_vel, point_vel,
                        &aer_vel_view);

// Reconstruct from an AerVelocityView.
EcefVelocity reconstructed;
stat = VelocityFrom(origin_loc, point_loc, origin_vel, aer_vel_view,
                    &reconstructed);

// Alternatively, pass a LocationView in place of point_loc.
AerLocationView aer_loc = ViewFrom<AerLocationView>(origin_loc, point_loc);
stat = VelocityViewFrom(origin_loc, aer_loc, origin_vel, point_vel,
                        &aer_vel_view);
stat = VelocityFrom(origin_loc, aer_loc, origin_vel, aer_vel_view,
                    &reconstructed);

// Variant dispatch.
VelocityTypes any_origin = EcefVelocity{10.0, 20.0, 30.0};
VelocityTypes any_point  = EcefVelocity{15.0, 18.0, 35.0};
EcefVelocityView variant_view;
stat = VelocityViewFrom(origin_loc, point_loc, any_origin, any_point,
                        &variant_view);

VelocityViewTypes any_vel_view = EcefVelocityView{5.0, -2.0, 5.0};
EcefVelocity ecef_out;
stat = VelocityFrom(origin_loc, point_loc, any_origin, any_vel_view, &ecef_out);
```

## Installation

### Conan (recommended)

```bash
conan install --requires="mutatio/0.1.0" --build=missing
```

Then in your `CMakeLists.txt`:

```cmake
find_package(mutatio REQUIRED)
target_link_libraries(my_target PRIVATE mutatio::mutatio)
```


## Quick Start

**CMake + Conan:**
```bash
conan build . --build=missing                    # install deps + generate toolchain

cmake --preset conan-release -DENABLE_TESTING=ON # reconfigure with tests enabled
cmake --build build/Release --target test        # build and run tests

cmake --preset conan-release -DENABLE_TESTING=ON -DENABLE_COVERAGE=ON  # enable coverage
cmake --build build/Release --target coverage    # generate coverage report

cmake --build build/Release --target format      # format sources
```

**Bazel:**
```bash
bazel test //mutatio:all_tests                       # run all tests
bazel test //mutatio:location_exchange_test          # run one test
bazel run //:format                                  # format sources
bazel run //:check_format                            # check formatting
```

**Create and test the Conan package locally:**
```bash
conan create . --build=missing
```
