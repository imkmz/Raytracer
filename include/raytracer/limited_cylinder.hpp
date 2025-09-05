/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** limited_cylinder
*/

#pragma once
#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>

class LimitedCylinder : public Hittable {
    public:
        LimitedCylinder() {}
        LimitedCylinder(Math::Vector3D base_center, Math::Vector3D axis, double radius, double min_height, double max_height, std::shared_ptr<Material> m)
            : base_center(base_center), axis(axis.normalized()), radius(radius), min_height(min_height), max_height(max_height), mat_ptr(m) {
            min_center = base_center + axis * min_height;
            max_center = base_center + axis * max_height;
        };

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        Math::Vector3D base_center;
        Math::Vector3D axis;
        double radius;
        double min_height;
        double max_height;
        Math::Vector3D min_center;
        Math::Vector3D max_center;
        std::shared_ptr<Material> mat_ptr;

        bool hit_base(const Ray& r, double t_min, double t_max, HitRecord& rec, bool is_top_base) const;
}; 