#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "utility.h"

class Sphere : public Hittable {
public:
    Point3 center;
    double radius;
    shared_ptr<Material> mat_ptr;

    Sphere() {}
    Sphere(Point3 c, double r, shared_ptr<Material> m) : center(c), radius(r), mat_ptr(m) {}

    // Returns whether ray hit sphere and populates pass-by-reference hit_record
    virtual bool hit(const Ray&r, double t_min, double t_max, hit_record& rec) const override;
};

bool Sphere::hit(const Ray&r, double t_min, double t_max, hit_record& rec) const {
    double a = r.direction().length_squared();
    Vec3 vec_ac = r.origin() - center;
    double half_b = dot(r.direction(), vec_ac);
    double c = vec_ac.length_squared() - (radius*radius);
    double discriminant = half_b*half_b - a*c;

    // Roots are invalid (complex) if discrimant is negative
    if (discriminant < 0) {
        return false;
    } 

    // Roots are invalid if outside of t_min and t_max
    double sqrt_discriminant = std::sqrt(discriminant);
    double root = (-half_b - sqrt_discriminant) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    // t value used for ray
    rec.t = root;
    // Point on sphere that ray hits
    rec.p = r.at(rec.t);
    // Normalized outward normal of hit
    Vec3 outward_normal = (rec.p - center) / radius;
    // Sets hit record with whether ray is inward or outward for coloring
    // and sets surface normal to always be opposite to ray direction
    rec.set_face_normal(r, outward_normal);
    // Set hit record material pointer to point to material of sphere
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif
