#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Point3 orig;
    Vec3 dir;

    Ray() {}

    Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    ~Ray() {}

    Point3 origin() const { return orig; }

    Vec3 direction() const { return dir; }

    // Returns the point where the ray reaches with variable t
    Point3 at(double t) const {
        return (orig + t*dir);
    }
};

#endif
