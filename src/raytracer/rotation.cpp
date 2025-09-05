/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - B-OOP-400-STG-4-1-raytracer-mael.kempf-le-pape                     *
 * Description     -                                                                    *
 *     rotation                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "raytracer/rotation.hpp"

bool Rotation::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    Math::Vector3D origin = rotateVectorInverse(r.origin());
    Math::Vector3D direction = rotateVectorInverse(r.direction());

    Ray rotated_ray(origin, direction);
    if (!object->hit(rotated_ray, t_min, t_max, rec))
        return false;
    rec.p = rotateVector(rec.p);
    rec.normal = rotateVector(rec.normal);
    return true;
}

Math::Vector3D Rotation::rotateVector(const Math::Vector3D& v) const
{
    return v * cos_ + axis.cross(v) * sin_ + axis * axis.dot(v) * (1 - cos_);
}

Math::Vector3D Rotation::rotateVectorInverse(const Math::Vector3D& v) const
{
    return v * cos_ + axis.cross(v) * (-sin_) + axis * axis.dot(v) * (1 - cos_);
}