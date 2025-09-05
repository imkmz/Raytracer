/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** utils
*/

#pragma once
#include "math/vector3D.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/hittable.hpp"
#include <iostream>

namespace RaytracerUtils {
    Math::Vector3D ray_color(const Ray& r, const Hittable& world, int depth);
    void write_color(std::ostream &out, const Math::Vector3D& color, int samples_per_pixel);
}
