/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** Iintersectable
*/

#pragma once
#include "raytracer/ray.hpp"
#include "raytracer/Hitrecord.hpp"

class IIntersectable {
    public:
        virtual ~IIntersectable() = default;
        virtual bool intersects(const Ray& ray, HitRecord& rec) const = 0;
};
