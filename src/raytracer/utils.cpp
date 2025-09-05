/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** utils
*/

#include "raytracer/utils.hpp"
#include "raytracer/material.hpp"
#include <algorithm>

namespace RaytracerUtils {
    Math::Vector3D ray_color(const Ray& r, const Hittable& world, int depth) {
        HitRecord rec;

        if (depth <= 0)
            return Math::Vector3D(0,0,0);

        if (world.hit(r, 0.001, Math::infinity, rec)) {
            Ray scattered;
            Math::Vector3D attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, world, depth-1);
            return Math::Vector3D(0,0,0);
        }

        Math::Vector3D unit_direction = r.direction().normalized();
        auto t = 0.5*(unit_direction.y + 1.0);
        return (1.0-t)*Math::Vector3D(1.0, 1.0, 1.0) + t*Math::Vector3D(0.5, 0.7, 1.0);
    }

    void write_color(std::ostream &out, const Math::Vector3D& color, int samples_per_pixel) {
        auto r = color.x;
        auto g = color.y;
        auto b = color.z;

        auto scale = 1.0 / samples_per_pixel;
        r = std::sqrt(scale * r);
        g = std::sqrt(scale * g);
        b = std::sqrt(scale * b);

        out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
    }
} 