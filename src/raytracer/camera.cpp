/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** camera
*/

#include "raytracer/camera.hpp"
#include "math/vector3D.hpp"
#include <cmath>

Camera::Camera(
    Math::Vector3D lookfrom,
    Math::Vector3D lookat,
    Math::Vector3D vup,
    double vfov,
    double aspect_ratio,
    double aperture,
    double focus_dist
) {
    auto theta = Math::degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w = (lookfrom - lookat).normalized();
    u = vup.cross(w).normalized();
    v = w.cross(u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

    lens_radius = aperture / 2;
}

Ray Camera::get_ray(double s, double t) const {
    Math::Vector3D rd = lens_radius * Math::Vector3D::random_in_unit_disk();
    Math::Vector3D offset = u * rd.x + v * rd.y;

    return Ray(
        origin + offset,
        lower_left_corner + s*horizontal + t*vertical - origin - offset
    );
} 