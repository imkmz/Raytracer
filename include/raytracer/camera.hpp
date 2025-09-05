/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** camera
*/

#pragma once
#include "ray.hpp"
#include "math/vector3D.hpp"

class Camera {
    public:
        Camera(
            Math::Vector3D lookfrom,
            Math::Vector3D lookat,
            Math::Vector3D vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist
        );

        Ray get_ray(double u, double v) const;

    private:
        Math::Vector3D origin;
        Math::Vector3D lower_left_corner;
        Math::Vector3D horizontal;
        Math::Vector3D vertical;
        Math::Vector3D u, v, w;
        double lens_radius;
};
