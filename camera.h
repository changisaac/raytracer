#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

/*
    Class representing a virtual camera and viewport in the scene. The viewport
    is centered 1 unit in the -z direction and its dimensions are based on the aspect
    ratio and vertical FOV of the camera. Image pixels are distributed across viewport.
*/

class Camera {
private:
    // Location of camera in world frame
    Point3 origin;
    // Upper left corner of viewport in world frame
    Point3 upper_left_corner;
    // Horizontal direction vector of viewport
    Vec3 horizontal;
    // Vertical direction vector of viewport
    Vec3 vertical;
    // Camera view coordinate frame basis
    Vec3 u, v, w;
    // Radius of camera lens
    double lens_radius;

public:
    /*
        Instantiates the camera with a vertical FOV and aspect ratio.

        @param look_from Location of camera origin in world frame
        @param look_at Location of where camera is looking at in world frame
        @param view_up Up vector of camera in world frame
        @param vertical_fov Vertical frame of view of the camera in degrees
        @param aspect_ratio Width over height ratio of the viewport in pixels
        @param aperture Aperture of camera lens (diameter of lens) in world frame units
        @param focus_dist Distance along w basis where objects are in perfect focus
    */
    Camera(Point3 look_from, Point3 look_at, Vec3 view_up, double vertical_fov,
           double aspect_ratio, double aperture, double focus_dist) {
        // Define viewport height and width and world coordinates
        // Viewport is centered at z = -1 from the origin
        double h = tan(degrees_to_radians(vertical_fov/2.0));
        double viewport_height = h*2.0;
        double viewport_width = viewport_height * aspect_ratio;

        // Create coordinate frame basis
        // w vector is analogous to z vector so viewport will be at w = -1
        // therefore w must point from look_at to look_from
        // View up vector is arbitrary and is not necessarily orthogonal to w
        // so must use cross product to get proper "up" or vertical vector
        // The point is that view_up is related and thus affects to vertical vector
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);

        origin = look_from;
        
        // Calculate viewport attributes
        // As long as w scales with horizontal and vertical the view will always look the same.
        // Move plane out to focus dist while scaling horizontal and vertical so view
        // stays the same.
        horizontal = viewport_width * focus_dist * u;
        vertical = viewport_height * focus_dist * v;
        upper_left_corner = origin - horizontal/2 + vertical/2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    /*
        Returns Ray originating from origin going through specified location in viewport.
    */
    Ray get_ray(double s, double t) const {
        // Get random points within the lens radius
        Vec3 random = lens_radius * random_in_unit_disk();
        // Apply as offset in u and v directions from origin
        Vec3 offset = random.x()*u + random.y()*v;
        Point3 offset_origin = origin + offset;

        // Points at the focus distance away from the origin will focus onto the
        // same point every time since the ray below is define to go from offset_origin
        // to point on plane defined by vertical and horizontal.
        // Points behind or in front of the docus distance will hit a slightly different
        // point each time since the offset origin is randomized so the ray will be at a
        // slightly different angle before and after the focus distance
        return Ray(offset_origin,
                   (upper_left_corner + s*horizontal - (1-t)*vertical) - offset_origin);
    }
};

#endif
