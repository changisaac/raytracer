#include "utility.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

// Return color of pixel based on ray and scene
Color ray_color(const Ray& r, const HittableList& scene, int depth) {
    if (depth <= 0) {
        return Color(0, 0, 0);
    }

    hit_record rec;
    if (scene.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, scene, depth-1);
        }
        return Color(0, 0, 0);
    }

    // If didn't hit anything in scene then just color blue to white gradient
    Vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + (t * Color(0.5, 0.7, 1.0));
}

HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

HittableList metal_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000.5,0), 1000, ground_material));

    auto metal = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<Sphere>(Point3(0.0, 0.0, 0.0), 0.5, metal));
    world.add(make_shared<Sphere>(Point3(1.25, 0.0, 0.0), 0.5, metal));
    world.add(make_shared<Sphere>(Point3(-1.25, 0.0, 0.0), 0.5, metal));

    world.add(make_shared<Sphere>(Point3(0.0, 0.75, 0.5), 0.2, metal));
    world.add(make_shared<Sphere>(Point3(-0.75, 0.75, 0.5), 0.2, metal));
    world.add(make_shared<Sphere>(Point3(0.75, 0.75, 0.5), 0.2, metal));

    return world;
}

HittableList glass_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000.5,0), 1000, ground_material));

    auto glass = make_shared<Dielectric>(1.5);
    auto metal = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<Sphere>(Point3(0.0, 0.0, 0.0), 0.5, glass));
    world.add(make_shared<Sphere>(Point3(1.25, 0.0, 0.0), 0.5, glass));
    world.add(make_shared<Sphere>(Point3(-1.25, 0.0, 0.0), 0.5, glass));

    world.add(make_shared<Sphere>(Point3(0.0, -0.3, -1.0), 0.2, metal));
    world.add(make_shared<Sphere>(Point3(-1.25, -0.3, -1.0), 0.2, metal));
    world.add(make_shared<Sphere>(Point3(1.25, -0.3, -1.0), 0.2, metal));

    return world;
}

int main() {
    // Image properties
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1280;
    const int image_height = static_cast<double>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 40;

    /*
    // Scene properties
    HittableList scene;
    
    // Lambertian (diffuse) material which reflects 80% of red and green and absorbs all blue
    // OR in other words, attenuates r and g by 20% and attnuates blue by 100%
    // Therefore this material will appear yellow
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    scene.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    scene.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.5), 100.0, material_ground));
    scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
    scene.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
    */

    HittableList scene = glass_scene();

    // Camera properties
    Point3 look_from(4, 1, -0.6);
    Point3 look_at(0, 0.0, -0.6);
    //Point3 look_from(0, 0, 2.5);
    //Point3 look_at(0, 0.0, 0);
    Vec3 view_up(0, 1, 0);
    double focus_dist = 10.0;
    double aperture = 0.0;
    double vertical_fov_deg = 55.0;

    Camera cam(look_from,look_at, view_up, vertical_fov_deg, aspect_ratio, aperture, focus_dist);

    // Render scene

    // Write to PPM image file
    // P3 means colors are in ascii and 255 is max color value
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Per row from top to bottom
    for (int j=image_height-1 ; j>=0 ; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        // Per column from left to right
        for (int i=0 ; i<image_width ; ++i) {
            Color pixel_color = Color(0, 0, 0);

            // For each pixel shoot multiple rays which vary randomly by max one pixel
            // then aggregate the pixel colors of all sampls and divide by number of samples
            // to get antialiasing in pixel coloring, results in overall more uniform shading
            for (int k=0 ; k<samples_per_pixel ; ++k) {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, scene, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
