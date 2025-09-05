/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** triangle
*/

#pragma once
#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>

class Triangle : public Hittable {
public:
    Triangle() {}
    Triangle(Math::Vector3D a, Math::Vector3D b, Math::Vector3D c, std::shared_ptr<Material> m)
        : left(a), right(b), top(c), mat_ptr(m) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    Math::Vector3D left;
    Math::Vector3D right;
    Math::Vector3D top;
    std::shared_ptr<Material> mat_ptr;
};
