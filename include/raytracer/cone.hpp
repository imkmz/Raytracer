/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** cone
*/

#pragma once
#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>

class Cone : public Hittable {
    public:
        Cone() {}
        Cone(Math::Vector3D apex, Math::Vector3D axis, double angle, double height, std::shared_ptr<Material> m)
            : apex(apex), axis(axis.normalized()), angle(angle), height(height), mat_ptr(m) {
            base_center = apex + axis * height;
            base_radius = height * tan(angle);
        };

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        Math::Vector3D apex;
        Math::Vector3D axis;
        double angle;
        double height;
        Math::Vector3D base_center;
        double base_radius;
        std::shared_ptr<Material> mat_ptr;

        bool hit_base(const Ray& r, double t_min, double t_max, HitRecord& rec) const;
}; 