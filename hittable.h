#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "utility.h"

class Material;

struct hit_record {
    Point3 p;
    Vec3 normal;
    shared_ptr<Material> mat_ptr;
    double t;
    bool inward;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        // If ray direction is opposite to outward normal (neg dot product) then ray is
        // pointing in from the outside
        inward = (dot(r.direction(), outward_normal) < 0);
        // Make normal always opposite direction to ray
        normal = inward ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
