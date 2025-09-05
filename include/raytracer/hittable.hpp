/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** hittable
*/

#pragma once
#include "ray.hpp"
#include "math/vector3D.hpp"
#include <memory>

class Material;

struct HitRecord {
    Math::Vector3D p;
    Math::Vector3D normal;
    std::shared_ptr<Material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Math::Vector3D& outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};
