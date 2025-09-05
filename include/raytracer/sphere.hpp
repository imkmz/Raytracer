/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** sphere
*/

#pragma once
#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Math::Vector3D cen, double r, std::shared_ptr<Material> m)
            : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        Math::Vector3D center;
        double radius;
        std::shared_ptr<Material> mat_ptr;
};
