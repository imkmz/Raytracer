/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** triangle
*/

#include "raytracer/triangle.hpp"
#include <cmath>

bool Triangle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    Math::Vector3D edge1 = right - left;
    Math::Vector3D edge2 = top - left;
    Math::Vector3D h = r.direction().cross(edge2);
    Math::Vector3D s = r.origin() - left;
    Math::Vector3D q = s.cross(edge1);
    double a = edge1.dot(h);
    double f = 1.0 / a;
    double u = f * s.dot(h);
    double v = f * r.direction().dot(q);
    double t = f * edge2.dot(q);

    if (a > -0.00001 && a < 0.00001)
        return false;
    if (u < 0 || u > 1)
        return false;
    if (v < 0 || u + v > 1)
        return false;
    if (t < t_min || t > t_max)
        return false;

    rec.t = t;
    rec.p = r.at(t);
    Math::Vector3D outward_normal = edge1.cross(edge2).normalized();
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}