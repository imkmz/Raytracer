/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** limited_cone
*/

#pragma once
#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>

class LimitedCone : public Hittable {
    public:
        LimitedCone() {}
        LimitedCone(Math::Vector3D base_center, Math::Vector3D axis, double angle, double min_height, double max_height, std::shared_ptr<Material> m)
            : base_center(base_center), axis(axis.normalized()), angle(angle), min_height(min_height), max_height(max_height), mat_ptr(m) {
            apex = base_center - axis * max_height;
            min_center = base_center - axis * (max_height - min_height);
            min_radius = min_height * tan(angle);
            max_radius = max_height * tan(angle);
        };

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        Math::Vector3D base_center;
        Math::Vector3D apex;
        Math::Vector3D axis;
        double angle;
        double min_height;
        double max_height;
        Math::Vector3D min_center;
        double min_radius;
        double max_radius;
        std::shared_ptr<Material> mat_ptr;

        bool hit_base(const Ray& r, double t_min, double t_max, HitRecord& rec, bool is_top_base) const;
}; 