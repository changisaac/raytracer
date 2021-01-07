#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

/*
    A class representing a vector with 3 double elements. Implements
    attributes and operator overrides.
*/

class Vec3 {
public:
    double v[3];

    Vec3() : v{0, 0, 0} {}

    Vec3(double v0, double v1, double v2) : v{v0, v1, v2} {}

    ~Vec3() {}

    double x() const { return v[0]; }

    double y() const { return v[1]; }

    double z() const { return v[2]; }

    double length_squared() const {
        return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    // Return true if vector is close to zero in all dimensions
    bool near_zero() const {
        const double s = 1e-8;
        return (fabs(v[0]) < s) & (fabs(v[1]) < s) & (fabs(v[2]) < s);
    }

    // Random Utility functions

    inline static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3 random(double min, double max) {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    // Operator overloads

    double operator[](int i) const {
        return v[i];
    }

    double& operator[](int i) {
        return v[i];
    }

    Vec3 operator-() const {
        return Vec3(-v[0], -v[1], -v[2]);
    }

    Vec3& operator/=(const double sca) {
        return *this *= 1/sca;
    }

    Vec3& operator+=(const Vec3& vec) {
        v[0] += vec[0];
        v[1] += vec[1];
        v[2] += vec[2];
        return *this;
    }

    Vec3& operator*=(const double sca) {
        v[0] *= sca;
        v[1] *= sca;
        v[2] *= sca;
        return *this;
    }
};

// Type aliases for Vec3
using Point3 = Vec3;
using Color = Vec3;

// Vec3 math utility functions

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.v[0] << ' ' << v.v[1] << ' ' << v.v[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.v[0] + v.v[0], u.v[1] + v.v[1], u.v[2] + v.v[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.v[0] - v.v[0], u.v[1] - v.v[1], u.v[2] - v.v[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.v[0] * v.v[0], u.v[1] * v.v[1], u.v[2] * v.v[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t*v.v[0], t*v.v[1], t*v.v[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.v[0] * v.v[0] + u.v[1] * v.v[1] + u.v[2] * v.v[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.v[1] * v.v[2] - u.v[2] * v.v[1],
                u.v[2] * v.v[0] - u.v[0] * v.v[2],
                u.v[0] * v.v[1] - u.v[1] * v.v[0]);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

// Returns a random vector within the unit sphere from the origin
Vec3 random_in_unit_sphere() {
    while (true) {
        // Generate random vector in -1 to 1 box
        Vec3 n = Vec3::random(-1, 1);
        // Keep generating until vector has length less than 1
        // Can disregard square root since equivalent as just squaring
        if (n.length_squared() >= 1) continue;
        return n;
    }
}

Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// Returns random vector in unit disk from origin with length smaller than 1
Vec3 random_in_unit_disk() {
    while (true) {
        Vec3 v = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        // Keep generating until vector has length less than 1
        if (v.length_squared() >= 1) continue;
        return v;
    }
}

// Returns the reflected ray direction given an incoming ray and normal
Vec3 reflect(const Vec3& v, const Vec3& n) {
    // Since v is incoming and n is always opposite against v then b needs to be negated
    Vec3 b = -dot(v, n) * n;
    return v + 2*b;
}

// Returns the refracted ray direction given an incoming ray, normal, and eta incident and
// refracting eta
// eta ratio is the eta ratio between the incident ray medium and the refracted ray medium
Vec3 refract(const Vec3& v, const Vec3& n, double eta_ratio) {
    double cos_theta = fmin(dot(-v, n), 1.0);
    Vec3 ray_perp = eta_ratio * (v + cos_theta*n);
    Vec3 ray_parallel = -std::sqrt(fabs(1.0 - ray_perp.length_squared())) * n;
    return ray_perp + ray_parallel;
}

#endif
