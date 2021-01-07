#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"

/*
    Abstract class representing a material type of an object. Implements functions for how a
    a ray would interact with the material such as how it would scatter.
*/
class Material {
public:
    // Given an incoming ray and hit record populate the resulting scattered ray
    virtual bool scatter(const  Ray& incoming, const hit_record& rec, Color& attenuation,
                         Ray& scattered) const = 0;
};

/*
    Lambertian (diffuse) material class
*/
class Lambertian : public Material {
public:
    // albedo = reflected_light / incident_light
    // The amount of albedo for each red, green, and blue will define the color of the material
    Color albedo;

    Lambertian(const Color& a) : albedo(a) {}

    virtual bool scatter(const Ray& incoming, const hit_record& rec, Color& attenuation,
                         Ray& scattered) const override {
        // Ray direction will be scattered in unit circle surface, tangent to hit point
        auto scatter_direction = rec.normal + random_unit_vector();

        // Disregard scatter direction if length close to zero
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        // Scattered ray will originate from hit point in scatter direction
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

// Metal or totally reflective material
class Metal : public Material {
public:
    Color albedo;
    // Fuzziness of metal, phenomena of randomized direction of reflection
    // irregular metal surfaces, capped at 1
    // Higher fuzz values correspond to larger variation from specular reflection
    double fuzz;

    Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& incoming, const hit_record& rec, Color& attenuation,
                         Ray& scattered) const override {
        auto reflected_direction = reflect(unit_vector(incoming.direction()), rec.normal);
        scattered = Ray(rec.p, reflected_direction + fuzz*random_in_unit_sphere());
        attenuation = albedo;

        // Return final check if scattered (reflected) ray has no components opposite to normal
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

// Dialectrics/non-metals or materials which refract light when possible
class Dielectric : public Material {
public:
    double refraction_index;

    Dielectric(double index) : refraction_index(index) {}

    virtual bool scatter(const  Ray& incoming, const hit_record& rec, Color& attenuation,
                         Ray& scattered) const override{
        attenuation = Color(1.0, 1.0, 1.0);

        // Consider outside medium is always air for now so refraction index of 1.0
        double eta_ratio;
        if (rec.inward) {
            // air into object
            eta_ratio =  1.0/refraction_index;
        } else {
            // object into air
            eta_ratio = refraction_index;
        }

        // Calculate direction of refracted ray in material
        Vec3 unit_incoming_dir= unit_vector(incoming.direction());

        // If no solution to Snell's Law (total internal reflection) then reflect ray
        double cos_theta = fmin(dot(-unit_incoming_dir, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = (eta_ratio * sin_theta) > 1.0;

        Vec3 out_direction;
        // If eta_ratio*sin theta is above 1.0 then no refraction is possible so reflect ray
        // Use Schlick Approx. for reflective surface at different angles
        if (cannot_refract || reflectance(cos_theta, eta_ratio) > random_double()) {
            out_direction = reflect(unit_incoming_dir, rec.normal);
        } else {
            out_direction = refract(unit_incoming_dir, rec.normal, eta_ratio);
        }

        // Calculate final refracted ray
        scattered = Ray(rec.p, out_direction);
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

#endif
