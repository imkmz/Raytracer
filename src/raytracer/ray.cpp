/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** ray
*/

#include "raytracer/ray.hpp"

Ray::Ray() {}

Ray::Ray(const Math::Vector3D& origin_, const Math::Vector3D& direction_)
    : orig(origin_), dir(direction_) {}

Math::Vector3D Ray::origin() const
{
    return orig;
}

Math::Vector3D Ray::direction() const
{
    return dir;
}

Math::Vector3D Ray::at(double t) const
{
    return orig + dir * t;
}