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

```c++
#include "mutatio/velocity.h"
#include "mutatio/velocity_exchange.h"

...

// Define a velocity in the ECEF frame.
EcefVelocity ecef_vel{100.0, -50.0, 25.0};

// Convert to a NED velocity at a given location.
LlaLocation loc{33.0, 74.0, 1000.0};
auto ned_vel = VelocityFrom<NedVelocity>(ecef_vel, loc);

// Convert to a pre-allocated Velocity.
NedVelocity pre_alloc_vel;
auto stat = VelocityFrom(ecef_vel, loc, &pre_alloc_vel);

// Convert back to ECEF.
auto ecef_vel_out = VelocityFrom<EcefVelocity>(ned_vel, loc);

// Convert arbitrary Velocity and Location variants.
VelocityTypes any_vel = EcefVelocity{100.0, -50.0, 25.0};
LocationTypes any_loc = LlaLocation{33.0, 74.0, 1000.0};
NedVelocity specific_vel;
stat = VelocityFrom(any_vel, any_loc, &specific_vel);
```

### VelocityView

VelocityViews represent the relative velocity between an origin Velocity and
another Velocity with respect to a given coordinate frame.

* EcefVelocityView - The difference in meters per second between two velocities
  expressed in the ECEF coordinate frame.
* NedVelocityView - The difference in meters per second between two velocities
  expressed as North, East, and Down components. A reference Location is
  required to define the local NED frame orientation.

```c++
#include "mutatio/velocity_view.h"
#include "mutatio/velocity_view_exchange.h"

...

EcefVelocity origin_vel{10.0, 20.0, 30.0};
EcefVelocity point_vel{15.0, 18.0, 35.0};

// Construct a VelocityView between two velocities.
auto ecef_vel_view = VelocityViewFrom<EcefVelocityView>(origin_vel, point_vel);

// Construct a pre-allocated VelocityView.
EcefVelocityView pre_alloc_view;
auto stat = VelocityViewFrom(origin_vel, point_vel, &pre_alloc_view);

// Reconstruct a Velocity from an origin Velocity and a VelocityView.
auto point_vel_out = VelocityFrom<EcefVelocity>(origin_vel, ecef_vel_view);

// NedVelocityView requires a reference Location to define the NED frame.
LlaLocation loc{33.0, 74.0, 1000.0};
auto ned_vel_view = VelocityViewFrom<NedVelocityView>(origin_vel, point_vel, loc);

// Construct a pre-allocated NedVelocityView.
NedVelocityView pre_alloc_ned_view;
stat = VelocityViewFrom(origin_vel, point_vel, loc, &pre_alloc_ned_view);

// Reconstruct a Velocity from an origin Velocity and a NedVelocityView.
auto point_vel_out2 = VelocityFrom<EcefVelocity>(origin_vel, ned_vel_view, loc);

// NED-frame origin and point velocities can also be used directly.
NedVelocity ned_origin{10.0, 20.0, 30.0};
NedVelocity ned_point{11.0, 22.0, 33.0};
auto ned_view2 = VelocityViewFrom<NedVelocityView>(ned_origin, ned_point, loc);
auto ned_point_out = VelocityFrom<NedVelocity>(ned_origin, ned_view2, loc);

// ... or using arbitrary Velocity and VelocityView variants.
VelocityTypes any_origin       = EcefVelocity{10.0, 20.0, 30.0};
VelocityViewTypes any_vel_view = EcefVelocityView{5.0, -2.0, 5.0};
EcefVelocity specific_vel;
stat = VelocityFrom(any_origin, any_vel_view, &specific_vel);

// Variant dispatch with NedVelocityView also accepts a location.
VelocityViewTypes ned_view_variant = NedVelocityView{1.0, 2.0, 3.0};
stat = VelocityFrom(any_origin, ned_view_variant, loc, &specific_vel);
```

## Quick Start

```bash
conan build . --build=missing
cmake --build build/Release --target coverage
cmake --build build/Release --target format
```
