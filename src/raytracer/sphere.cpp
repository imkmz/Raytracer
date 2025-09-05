/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** sphere
*/

#include "raytracer/sphere.hpp"
#include <cmath>

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Math::Vector3D oc = r.origin() - center;
    auto a = r.direction().length() * r.direction().length();
    auto half_b = oc.dot(r.direction());
    auto c = oc.length() * oc.length() - radius * radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Math::Vector3D outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}