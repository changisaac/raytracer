#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns random double within [0, 1)
inline double random_double() {
    return rand() / ((double)RAND_MAX + 1.0);
}

// Returns random double within [min, max)
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;

    return x;
}

// Usings

using std::shared_ptr;
using std::make_shared;
using std::tan;

// Common Headers

#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include "material.h"

#endif